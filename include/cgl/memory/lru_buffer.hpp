#ifndef LRU_BUFFER_H_201501160255
#define LRU_BUFFER_H_201501160255

#include <cgl/public.h>
#include <vector>

namespace cgl{

template<typename T>
class lru_cache;

template<typename T>
class dual_cache;

//���ݽṹ
//lru_data<T>
template<typename T>
class lru_buffer
{
public:
    typedef T value_type;
	typedef size_t size_type;
    const static size_t INVALID_ID = INT16_MAX;

public:
    fsize_t   addr;
    size_type prev;
    size_type next;

    uint32_t   user;
    value_type value;

//private:
public:

    friend class lru_cache<T>;
    friend class dual_cache<T>;

public:
    lru_buffer() : addr(FILE_SIZE_MAX), prev(INVALID_ID), next(INVALID_ID), user(), value()
    {
        //memset(data, 0, BUFFER_SIZE);
    }

    fsize_t address()const
    {
        return addr;
    }
};

template<typename T>
class lru_cache
{
public:
    typedef lru_cache this_type;
    typedef lru_buffer<T> buffer_type;
    const static size_t INVALID_ID = buffer_type::INVALID_ID;

    //��������Ҫ�Ƴ�����ʱ�������¼�
    //instance      a
    //buffer
    //new_address : -1 invalid_address
    typedef int (*function_type)(void* instance, buffer_type* buffer, fsize_t new_address);
    void* instance;
    function_type on_update;

private:
    std::vector<buffer_type> m_buffers;    //�������б�
    size_t m_main;

public:
    lru_cache(size_t size = 8) :
        instance(null),
        on_update(null),
        m_buffers(),
        m_main()
    {
        this->init(size);
    }

    //�������ջ�����
    size_t init(size_t size)
    {
        if(size == 0)size = 1;    //min size = 1
        m_main = 0;
        m_buffers.resize(size);
        for(size_t i=0; i < m_buffers.size(); ++i){
            m_buffers[i].addr = FILE_SIZE_MAX;
            m_buffers[i].prev = i - 1;
            m_buffers[i].next = i + 1;
        }
        m_buffers.front().prev = INVALID_ID;
        m_buffers.back().next = INVALID_ID;
        return size;
    }

    size_t size()const
    {
        return m_buffers.size();
    }

    buffer_type& buffer(size_t id)
    {
        return m_buffers[id];
    }

    buffer_type& operator[](size_t id)
    {
        return buffer(id);
    }

    //���뻺�棬���ػ����ַ��last���ػ��浱ǰ��ַ
    size_t allocate(fsize_t address, fsize_t* last = null)
    {
        TYPENAME buffer_type::size_type id = m_main;
        for(size_t i=0; i < this->size(); ++i){
            if(buffer(id).addr == address){
                break;
            }
            if(buffer(id).next == INVALID_ID){
                break;
            }
            id = buffer(id).next;
        }

        //ƥ�䵽�ˣ����е���
        if(last){
            (*last) = buffer(id).addr;
        }

        if(id == m_main){
            #ifdef _DEBUG
            if(buffer(id).address() != address){
                CGL_ASSERT(0);
            }
            #endif
            return id;
        }

        if(buffer(id).prev != INVALID_ID){
            buffer(buffer(id).prev).next = buffer(id).next;
        }
        if(buffer(id).next != INVALID_ID){
            buffer(buffer(id).next).prev = buffer(id).prev;
        }
        buffer(id).prev = INVALID_ID;
        buffer(id).next = m_main;
        buffer(id).addr = address;
        buffer(m_main).prev = id;
        m_main = id;
        return id;
    }

    buffer_type* find(fsize_t address)
    {
        for(size_t i=0; i<m_buffers.size(); ++i){
            if(m_buffers[i].address() == address){
                return &m_buffers[i];
            }
        }
        return null;
    }
};

/*
LRU�������������ݴ�����ʱ���õĵ����򵥵��㷨�ͱȽϺõ�Ч�ʣ�������㷨�Ƚ�ʵ�õ�ԭ��
�������LRU��ʵ�֣�ʹ�õ�˫����������������Ҳ��ʵ�֣�ֻ�Ǹ����һЩ�����л������֯��
��vector���ڣ��洢������������ͬʹ�õĻ���ռ䣬�²����Ļ��������ڸ������б����棬
���һ����ַ�ڸ������б����ٴ����У�����ߵ����������棬ͬʱ������β���Ļ�������ƶ���
������ͷ�������������б��Ĳ�������һ���㷨���棬һ����Ļ�������档oracle�����ݿ�Ҳʹ��
����������LRU�����Ļ��棬���������Ļ��棬�ȼ���ָ�Ч��ͬʱ�ֱ�����һ����LRU�����ݵ�ȱ�㡣
2016-2-22 0:05
*/

//7:29 2018/9/21
template<typename T>
class dual_cache
{
public:
    typedef dual_cache this_type;
    typedef lru_buffer<SIZE> buffer_type;
    const static uint32_t INVALID_ID = INT16_MAX;
private:
    size_t m_main;     //��������ʼ����
    size_t m_auxiliary;//����������ʼ����
    size_t m_main_size;//�������С
    //size_t m_aux_size; //���������С
    std::vector<buffer_type> m_caches;
public:
    dual_cache():m_main(), m_auxiliary(), m_main_size(), m_caches()
    {

    }

    dual_cache(size_t main_size, size_t aux_size = 0)
    {
        init(main_size, aux_size);
    }

    void init(size_t main_size, size_t aux_size)
    {
        if(main_size<4)main_size = 4;
        if(aux_size<4)aux_size = 4;

        m_main_size = main_size;
        //m_aux_size = aux_size;
        m_main = 0;
        m_auxiliary = m_main_size;
        m_caches.resize(main_size + aux_size);

        //��ʼ������
        for(size_t i=0; i < main_size + aux_size; ++i){
            m_caches[i].addr = FILE_SIZE_MAX;
            m_caches[i].prev = i - 1;
            m_caches[i].next = i + 1;
        }
        m_caches.front().prev = INVALID_ID;
        m_caches.back().next = INVALID_ID;
    }

    size_t size()const
    {
        return m_caches.size();
    }

    buffer_type& operator[](size_t id)
    {
        return m_caches[id];
    }

    buffer_type& cache(size_t id)
    {
        return m_caches[id];
    }

    //���ݵ�ַƥ��һ����������last�����������������ʹ�õĵ�ַ
    size_t allocate(fsize_t address, fsize_t* last = 0)
    {
        CGL_ASSERT(size());
        size_t id = m_main;
        bool is_main = false;
        for(size_t i=0; i < this->size(); ++i){
            if(cache(id).addr == address){
                if(i < m_main_size)is_main = true;
                break;
            }
            if(cache(id).next == INVALID_ID){//û��ƥ�䵽���ƶ�β���Ļ�������������ͷ��
                if(last)(*last) = cache(id).addr;
                cache(cache(id).prev).next = INVALID_ID;
                cache(id).prev = cache(m_auxiliary).prev;
                cache(id).next = m_auxiliary;
                cache(id).addr = address;
                cache(cache(m_auxiliary).prev).next = id;
                cache(m_auxiliary).prev = id;
                m_auxiliary = id;
                return id;
            }
            id = cache(id).next;
        }

        //ƥ�䵽�ˣ����е���
        if(is_main){
            if(id == m_main){
                return id;
            }
        }
        else{
            m_auxiliary = cache(m_auxiliary).prev;
        }

        cache(cache(id).prev).next = cache(id).next;
        if(cache(id).next != INVALID_ID){
            cache(cache(id).next).prev = cache(id).prev;
        }
        cache(id).prev = INVALID_ID;
        cache(id).next = m_main;
        cache(m_main).prev = id;
        m_main = id;
        return id;
    }

    size_t find(fsize_t address)
    {
        for(size_t i=0; i < this->size(); ++i){
            if(cache(i).address() == address){
                return i;
            }
        }
        return INVALID_ID;
    }
};

}//end namespace cgl

#endif //LRU_BUFFER_H_201501160255

#if 0


template<size_t BUFSIZE>
class lru_buffer
{
public:
    typedef lru_buffer<BUFSIZE> this_type;
    typedef fsize_t index_type;
    struct buffer_type
    {
        fsize_t address;        //address of buffer
        int        user;            //user data
        byte_t  data[BUFSIZE];    //buffer

        void init()
        {
            address = FILE_SIZE_MAX;
            user = 0;
            memset(data, 0, BUFSIZE);
        }
    };
    const static size_t bufsize = BUFSIZE;

    //owner : sender object
    //buffer
    //new_address : -1 invalid_address
    typedef int (*function_type)(void* owner, buffer_type* buffer, fsize_t new_address);
    void* object;
    function_type on_update;
private:
    std::vector<size_t>            m_indices;    //lru˳���б�
    std::vector<buffer_type>    m_buffers;    //�������б�
public:
    lru_buffer(size_t size = 8) : object(null), on_update(null)
        , m_indices(), m_buffers()
    {
        this->resize(size);
    }

    buffer_type* buffer(fsize_t address)
    {
        std::vector<size_t>::iterator itr = m_indices.begin();
        for(; itr!=m_indices.end(); ++itr){
            if(m_buffers[*itr].address == address)
                break;
        }

        if(itr == m_indices.end()){
            --itr;
        }
        if(on_update){
            on_update(object, &m_buffers[*itr], address);
        }
        m_buffers[*itr].address = address;
        std::rotate(m_indices.begin(), itr, itr+1);
        return top();
    }

    buffer_type* top()
    {
        return &m_buffers[m_indices[0]];
    }

    size_t size()const
    {
        return m_buffers.size();
    }

    //�������ջ�����
    size_t resize(size_t size)
    {
        if(size == 0)size = 1;
        m_buffers.resize(size);
        m_indices.resize(size);
        for(int i=0; i<size; ++i)
        {
            m_indices[i] = i;
            m_buffers[i].init();
        }
        return size;
    }

    //����ʵ��˳��Ļ�����
    buffer_type* operator[](size_t id)
    {
        return &m_buffers[id];
    }
};

template<size_t BUFSIZE>
class lru_cache
{
public:
    typedef fsize_t fsize_type;
    const static size_t bufsize = BUFSIZE;
    const static uint32_t INVALID_ID = INT16_MAX;
    class element_type
    {
    public:
        friend class lru_cache<BUFSIZE>;
        uint16_t attrib;          //�û����Ƶ�����
        byte_t   buffer[bufsize]; //������
    private:
        fsize_type addr;
        uint16_t prev;
        uint16_t next;
    public:
        element_type():attrib(), addr(FILE_SIZE_MAX), prev(INVALID_ID), next(INVALID_ID)
        {
            memset(buffer, 0, bufsize);
        }
        fsize_type address()const
        {
            return addr;
        }
    };

private:
    std::vector<element_type> m_caches;
    size_t m_main;     //��������ʼ����
    size_t m_auxiliary;//����������ʼ����
    size_t m_main_size;//�������С
    //size_t m_aux_size; //���������С
public:
    lru_cache():m_main(), m_auxiliary(), m_main_size(), m_caches()
    {

    }
    lru_cache(size_t main_size, size_t aux_size = 0)
    {
        init_cache(main_size, aux_size);
    }

    void init_cache(size_t main_size, size_t aux_size)
    {
        size_t max_size;
        if(main_size<4)main_size = 4;
        if(aux_size<4)aux_size = 4;
        max_size = main_size + aux_size;

        m_main_size = main_size;
        //m_aux_size = aux_size;
        m_main = 0;
        m_auxiliary = m_main_size;
        m_caches.resize(max_size);

        //��ʼ������
        for(size_t i=0; i<max_size; ++i)
        {
            m_caches[i].prev = i - 1;
            m_caches[i].next = i + 1;
        }
        m_caches.front().prev = INVALID_ID;
        m_caches.back().next = INVALID_ID;
    }

    size_t size()const
    {
        return m_caches.size();
    }

    element_type& operator[](size_t id)
    {
        return m_caches[id];
    }

    //���ݵ�ַƥ��һ����������last�����������������ʹ�õĵ�ַ
    size_t alloc(fsize_t address, fsize_t* last)
    {
        CGL_ASSERT(size());
        size_t id = m_main;
        bool is_main = false;
        for(size_t i=0; i<size(); ++i)
        {
            if(element(id).addr == address)
            {
                if(i<m_main_size)is_main = true;
                break;
            }
            if(element(id).next == INVALID_ID)
            {//û��ƥ�䵽���ƶ�β���Ļ�������������ͷ��
                if(last)(*last)=element(id).addr;
                element(element(id).prev).next = INVALID_ID;
                element(id).prev = element(m_auxiliary).prev;
                element(id).next = m_auxiliary;
                element(id).addr = address;
                element(element(m_auxiliary).prev).next = id;
                element(m_auxiliary).prev = id;
                m_auxiliary = id;
                return id;
            }
            id = element(id).next;
        }
        //ƥ�䵽�ˣ����е���
        if(is_main)
        {
            if(id == m_main)return id;
        }
        else
        {
            m_auxiliary = element(m_auxiliary).prev;
        }
        element(element(id).prev).next = element(id).next;
        if(element(id).next != INVALID_ID)
        {
            element(element(id).next).prev = element(id).prev;
        }
        element(id).prev = INVALID_ID;
        element(id).next = m_main;
        element(m_main).prev = id;
        m_main = id;
        return id;
    }
private:
    element_type& element(size_t id)
    {
        return m_caches[id];
    }
};
#endif