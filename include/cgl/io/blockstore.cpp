//#ifndef BLOCK_BASE_CPP_201408052240
//#define BLOCK_BASE_CPP_201408052240

#include <cgl/log.hpp>
#include "blockstore.hpp"

namespace cgl{
namespace io{

storage::storage():m_io(), m_cache(64)
{

}

storage::~storage()
{
    this->close();
}

int storage::open(const char* fn, int mode, int share)
{
    return this->open(fn, NULL, mode, share);
}

int storage::open(const wchar_t* fn, int mode, int share)
{
    return this->open(NULL, fn, mode, share);
}

int storage::open(const char* afn, const wchar_t* wfn, int mode, int share)
{
    if(is_open()){
        this->close();
    }

    if(afn)
        m_io.open(afn, mode & (~(io_storage|io_stream/*|io_block*/)), share);
    else if(wfn)
        m_io.open(wfn, mode & (~(io_storage|io_stream/*|io_block*/)), share);
    else
        return BLOCK_ERROR;

    if(!m_io.is_open())
    {
        return BLOCK_ERROR_FILE_OPEN;
    }

    if((mode & io_create) || (mode & io_memory))
    {//新建文档
        this->format(BLOCK_MAX);
    }
    else
    {//检测文件格式
        if(m_io.size() < ROOT_SIZE){
            return BLOCK_ERROR_FORMAT;
        }

        m_io.seek(0, seek_begin);
        m_io.read(&m_store, sizeof(m_store));
        if(blocks(ROOT_ELEMENT).type != BLOCK_TYPE_ROOT)
        {//文件格式错误
            this->close();
            return BLOCK_ERROR_FORMAT;
        }
    }
    return 0;
}

int storage::format(fsize_t capacity)
{
    m_store.root.init(BLOCK_TYPE_ROOT);
    m_store.root.size = ROOT_SIZE;    //end of store
    m_store.root.capacity = capacity;
    //memcpy(m_store.root.name, "ROOT_ELEMENT", 12);

    m_store.free.init(BLOCK_TYPE_FREE);
    //memcpy(m_store.free.name, "FREE_ELEMENT", 12);

    for(int i=0; i<256;++i){
        m_store.meta[i] = i;
    }

    m_io.seek(0, SEEK_SET);
    m_io.write(&m_store, sizeof(m_store));
    return 0;
}

void storage::close()
{
    m_io.close();
    memset(&m_store, 0, ROOT_SIZE);
}

bool storage::is_open()
{
    return m_io.is_open() && m_store.root.type == BLOCK_TYPE_ROOT;
}

int storage::get_info(fsize_t block, BLOCK_BASE* info)
{
    m_io.seek(block, SEEK_SET);
    return m_io.read(info, sizeof(BLOCK));
}

int storage::block_set(fsize_t block, const BLOCK_BASE* info)
{
    m_io.seek(block, SEEK_SET);
    return m_io.write(info, info->get_size());
}

BLOCK_BASE& storage::blocks(fsize_t address)
{
    if(address == ROOT_ELEMENT){
        return m_store.root;
    }
    else if(address == FREE_ELEMENT){
        return m_store.free;
    }

    fsize_t last;
    size_t n = m_cache.allocate(address, &last);
    if(last != address){
        //blockstore_log("block_get", tostr(address));
        memset(&m_cache[n].value, 0, sizeof(BLOCK));
        get_info(address, &m_cache[n].value);
    }

    return m_cache[n].value;
}

void storage::update(fsize_t address){
    if(address == ROOT_ELEMENT){
        block_set(address, &m_store.root);
        return ;
    }
    else if(address == FREE_ELEMENT){
        block_set(address, &m_store.free);
        return ;
    }

    buffer_type* buffer = m_cache.find(address);
    if(buffer){
        block_set(address, &buffer->value);
    }
}

int storage::set_name(fsize_t block, const char* name)
{
    BLOCK_BASE* b = &blocks(block);
    if(b->type == BLOCK_TYPE_LIST || b->type == BLOCK_TYPE_FILE){
        fsize_t n = this->find(b->root, name);
        if(n != 0){
            return n;
        }
        strncpy(b->name, name, BLOCK_MAX_NAME);
        fsize_t parent = b->root;
        remove(block);
        insert(parent, 0, block);
        return 0;
    }
    return BLOCK_IS_NOT_FILE;
}

int storage::set_ctime(fsize_t block, ftime_t time)
{
    blocks(block).ctime = time;
    update(block);
    return 0;
}

int storage::set_wtime(fsize_t block, ftime_t time)
{
    blocks(block).wtime = time;
    update(block);
    return 0;
}

fsize_t storage::read(fsize_t block, fsize_t seekpos, void* data, fsize_t size)
{
    if(is_list(block) || !size){
        return 0;
    }

    fsize_t block_size = this->size(block);

    if(block_size < seekpos){
        return 0;
    }

    if(block_size < size + seekpos){//获得实际可读数据
        size = block_size - seekpos;
    }

    if(!size)return 0;

    BLOCK_BASE* b = &blocks(block);
    while(b->capacity <= seekpos){//移动到当前读写块
        seekpos -= b->capacity;
        block = b->subnode;
        b = &blocks(block);
    }

    fsize_t read   = 0;
    fsize_t total  = 0;
    fsize_t offset = 0;
    byte_t* pdata  = static_cast<byte_t*>(data);

    do
    {
        block_size = b->capacity;
        block_size -= seekpos;
        if(block_size){
            offset = block + (b->type == BLOCK_TYPE_DATA ? sizeof(BLOCK_BASE) : sizeof(BLOCK)) + seekpos;
            m_io.seek(offset, SEEK_SET);
            read   = block_size < size ? block_size : size;
            read   = m_io.read(pdata, read);
            size  -= read;
            total += read;
            pdata += static_cast<size_t>(read);
            seekpos = 0;
        }
        block = b->subnode;
        b = &blocks(block);
        if(!block){
            //BLOCK_EOF
            break;
        }
    }while(size);
    return total;
}

fsize_t storage::write(fsize_t block, fsize_t seekpos, const void* data, fsize_t size)
{
    if(is_list(block)){
        return 0;
    }

    fsize_t block_size = this->size(block);
    if(block_size < seekpos + size)
    {//分配空间
        resize(block, seekpos + size);
    }

    block_size = capacity(block);

    BLOCK_BASE* b = &blocks(block);
    while(b->capacity <= seekpos)
    {//移动到当前读写块
        seekpos -= b->capacity;
        block = b->subnode;
        b = &blocks(block);
        if(!block){
            return BLOCK_EOF;
        }
    }

    fsize_t write  = 0;
    fsize_t total  = 0;
    fsize_t offset = 0;
    const byte_t* pdata = static_cast<const byte_t*>(data);

    do{
        block_size = b->capacity;
        block_size -= seekpos;
        if(block_size){
            offset = block + (b->type == BLOCK_TYPE_DATA ? sizeof(BLOCK_BASE) : sizeof(BLOCK)) + seekpos;
            m_io.seek(offset, SEEK_SET);
            write  = block_size < size ? block_size : size;
            write  = m_io.write(pdata, write);
            size  -= write;
            total += write;
            pdata += static_cast<int>(write);
            seekpos = 0;
        }
        block = b->subnode;
        b = &blocks(block);
        if(!block){
            //BLOCK_EOF
            break;
        }
    }while(size);
    return write;
}

fsize_t storage::size(fsize_t block)
{
    BLOCK_BASE* b = &blocks(block);
    if(block == ROOT_ELEMENT){//ROOT
        return b->size;
    }
    else if(b->type == BLOCK_TYPE_FILE){//文件
        return b->size;
    }
    else{//list, data, other
        return 0;
    }
}

fsize_t storage::capacity(fsize_t block)
{
    BLOCK_BASE* b = &blocks(block);
    if(block == ROOT_ELEMENT){//ROOT
        return b->capacity;
    }
    else if(b->type == BLOCK_TYPE_FILE){//文件
        fsize_t total = 0;
        do{
            total += b->capacity;
            block = b->subnode;
            b = &blocks(block);
        }while(block);
        return total;
    }
    else{//other
        return 0;
    }
}

// alloc(size)
// 分配一个 BLOCK[ size ] 的空间
//
fsize_t storage::alloc(fsize_t size)
{
    fsize_t block;
    BLOCK_BASE* b = &blocks(FREE_ELEMENT);
    block = b->subnode;
    while(block)
    {
        b = &blocks(block);
        if(size < b->capacity){
            if(size == 0){//maybe is list, need block.capacity == 0
                block = b->left;
                continue;
            }

            //如果自由空间大于申请空间的1.5倍，检查左树
            if(size*1.5 < b->capacity && b->left){
                fsize_t left = b->left;
                fsize_t left_size = blocks(left).capacity;    //size ?
                if(size < left_size){
                    block = left;
                }
                else{//分配成功
                    break;
                }
            }
            else
            {//没有再小的自由空间了，成功返回
                break;
            }
        }
        else if(size == b->capacity){
            break;
        }
        else{
            block = b->right;
        }
    }

    if(block)
    {
        remove(block);
    }
    else
    {
        //expand root.size
        fsize_t root_end = blocks(ROOT_ELEMENT).size;
        block = root_end;
        root_end += sizeof(BLOCK_BASE) + size;
        blocks(ROOT_ELEMENT).size = root_end;
        update(ROOT_ELEMENT);

        //init block
        b = &blocks(block);
        b->init(BLOCK_TYPE_DATA);
        b->capacity = size;
        update(block);
    }
    return block;
}

fsize_t storage::alloc(fsize_t parent, const char* name, fsize_t size, int attrib)
{
    if(!is_list(parent)){
        return BLOCK_IS_NOT_LIST;
    }

    fsize_t block = this->find(parent, name);
    if(block != 0){
        return block;
    }

    size = attrib & fa_directory ? 0 : size;
    fsize_t bsize = size_align(size + BLOCK_MAX_NAME);//add sizeof(block.name)

    block = alloc(bsize);
    BLOCK_BASE* b = &blocks(block);
    b->init((attrib & fa_directory) ? BLOCK_TYPE_LIST : BLOCK_TYPE_FILE);
    //info.attrib.value = attrib;
    b->size = size;
    b->capacity = bsize - BLOCK_MAX_NAME;
    strncpy(b->name, name, BLOCK_MAX_NAME);
    update(block);

    insert(parent, 0, block);
    return block;
}

fsize_t storage::resize(fsize_t block, fsize_t size)
{
    if(blocks(block).type != BLOCK_TYPE_FILE){//暂不支持文档文件的空间扩展
        return BLOCK_IS_NOT_FILE;
    }

    fsize_t total = this->capacity(block);

    if(total < size){//allocate space
        fsize_t bsize = size_align(size) - total;//need append size
        fsize_t node = block;
        BLOCK_BASE* b = &blocks(node);
        while(b->subnode){//move to last block
            node = b->subnode;
            b = &blocks(node);
        }
        fsize_t root_end = m_store.root.size;
        if(block_next(node) == root_end){//is end of store
            root_end  += bsize;
            m_store.root.size = root_end;
            update(ROOT_ELEMENT);
            b->capacity += bsize;
            update(node);
        }
        else{//append new block
            append(block, alloc(bsize));
        }
    }

    //update block info
    blocks(block).size = size;
    update(block);

    return size;
}

int storage::free(fsize_t block)
{
    if(block < ROOT_SIZE){
        return BLOCK_ERROR;
    }

    debug_log("free", string_t(block).c_str());

    //一个是先删除子项，再删除block。
    //一个是先删除block，再删除子项。
    //如果这时候出现io错误，结果是不一样的

    std::vector<fsize_t> ls;
    this->ls(ls, block, true);
    for(size_t i=0; i < ls.size(); ++i)
    {
        remove(ls[i]);
        insert(FREE_ELEMENT, 0, ls[i]);
    }

    //insert to free list
    remove(block);
    insert(FREE_ELEMENT, 0, block);

    return 0;
}

int storage::insert(fsize_t parent, fsize_t pnode, fsize_t block)
{
    if(!is_list(parent)){
        return BLOCK_IS_NOT_LIST;
    }

    BLOCK_BASE* b = &blocks(block);
    uint32_t type = b->type;
    fsize_t  block_size = b->capacity;
    if(parent == FREE_ELEMENT)
    {
        type = BLOCK_TYPE_FREE;
        //blocks(block).attrib.deleted = 1;
        if(b->type == BLOCK_TYPE_LIST || b->type == BLOCK_TYPE_FILE){
            block_size += BLOCK_MAX_NAME;//返还名字占用的空间
        }
    }
    else
    {
        //blocks(block).type = BLOCK_TYPE_FILE;
        //blocks(block).attrib.deleted = 0;
    }

    if(!blocks(parent).subnode){
        blocks(parent).subnode = block;
        update(parent);
        b->type = type;
        b->root = parent;
        b->capacity = block_size;
        update(block);
        //blockstore_log("insert : parent.child = ", tostr(block));
        return 0;
    }

    if(!pnode){
        pnode = blocks(parent).subnode;
    }

    bool is_less;
    while(pnode)
    {
        b = &blocks(pnode);
        if(parent == FREE_ELEMENT){
            is_less = block_size < b->capacity;
        }
        else{
            is_less = strncmp(blocks(block).name, b->name, BLOCK_MAX_NAME) < 0;
        }

        if(is_less){
            if(b->left){
                pnode = b->left;
                //blockstore_log(tostr(pnode), "insert : pnode left");
            }
            else{
                #ifdef _DEBUG
                if(pnode == block){
                    debug_log("insert error left", string_t(block).c_str());
                }
                #endif
                b->left = block;
                update(pnode);
                break;
            }
        }
        else{
            if(b->right){
                pnode = b->right;
                //blockstore_log("insert error right", tostr(block));
            }
            else{
                #ifdef _DEBUG
                if(pnode == block){
                    debug_log("insert error right", string_t(block).c_str());
                }
                #endif
                b->right = block;
                update(pnode);
                break;
            }
        }
    }

    b = &blocks(block);
    b->type = type;
    b->pnode = pnode;
    b->root = parent;
    b->capacity = block_size;
    update(block);
    return 0;
}

/*
将block从树中移除
成功返回    block
失败返回    0
可以优化
*/
int storage::remove(fsize_t block)
{
    if(block < ROOT_SIZE){
        return BLOCK_ERROR;
    }

    //blockstore_log("remove", tostr(block));

    BLOCK_BASE *b = &blocks(block);

    fsize_t parent = b->root;
    fsize_t pnode = b->pnode;
    fsize_t left = b->left;
    fsize_t right = b->right;

    b->root = 0;
    b->pnode = 0;
    b->left = 0;
    b->right = 0;
    update(block);

    if(block == blocks(parent).subnode){
        //set parent.child is 0
        blocks(parent).subnode = 0;
        update(parent);
    }

    if(pnode){
        b = &blocks(pnode);
        if(block == b->left){
            b->left = 0;
        }
        else{
            b->right = 0;
        }
        update(pnode);
    }

    if(left){
        blocks(left).pnode = 0;
        update(left);
        insert(parent, pnode, left);
    }

    if(right){
        blocks(right).pnode = 0;
        update(right);
        insert(parent, pnode, right);
    }
    return 0;
}

/*
把一个data block附加到parent后面
成功返回    0
失败返回    -1
*/
int storage::append(fsize_t parent, fsize_t block)
{
    if(blocks(parent).type != BLOCK_TYPE_FILE || blocks(block).type != BLOCK_TYPE_DATA)
    {
        return BLOCK_IS_NOT_FILE;
    }

    //blockstore_log("append : ", tostr(block));

    while(blocks(parent).subnode)
    {
        parent = blocks(parent).subnode;
    }
    blocks(parent).subnode = block;
    update(parent);

    blocks(block).type = BLOCK_TYPE_DATA;
    blocks(block).root = parent;
    update(block);
    return 0;
}

//成功返回block，失败返回负数，未找到返回0
fsize_t storage::find(fsize_t parent, const char* name)
{
    if(!is_list(parent)){
        return BLOCK_IS_NOT_LIST;
    }

    fsize_t block = blocks(parent).subnode;
    while(block){
        BLOCK_BASE* b = &blocks(block);
        int n = strncmp(name, b->get_name(), BLOCK_MAX_NAME);
        if(n == 0){
            break;
        }
        else{
            block = n < 0 ? b->left : b->right;
        }
    }
    return block;
}

void storage::enum_block(std::vector<fsize_t>& ls, fsize_t block, int mode)
{
    if(block){
        BLOCK_BASE* b;
        b = &blocks(block);
        if(b->type == BLOCK_TYPE_LIST){
            if(mode & BLOCK_LIST)ls.push_back(block);
        }
        else if(b->type == BLOCK_TYPE_FILE){
            if(mode & BLOCK_FILE)ls.push_back(block);
        }
        enum_block(ls, b->left, mode);
        enum_block(ls, b->right, mode);
        if((mode & BLOCK_CHILD) && b->type == BLOCK_TYPE_LIST){
            enum_block(ls, b->subnode, mode);
        }
    }
}

size_t storage::ls(std::vector<fsize_t>& ls, fsize_t block, int mode)
{
    if(!is_list(block)){
        return 0;
    }
    ls.clear();
    enum_block(ls, blocks(block).subnode, mode);
    return ls.size();
}

}//end namespace io
}//end namespace cgl

//#endif //BLOCK_BASE_CPP_201408052240
