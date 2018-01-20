/*

 xdocument.hpp

 sdragonx 2017-01-04 10:46:41

 一个类似json数据结构的配置文件

*/
#ifndef PSET_HPP_20170104104641
#define PSET_HPP_20170104104641

#include <cgl/string.hpp>
#include <cgl/hash/base64.hpp>

_CGL_BEGIN

enum xnode_type{
    node_unknow,
    node_object,
    node_array,
    node_string,
    //node_number,
    node_binary,
    node_comment,   //单行注释
    node_comments   //多行注释
};

class xdocument;
class xnode
{
public:
	typedef cstring<char> string_type;
	friend class xdocument;
    union value_type
    {
        xnode* node;    //name={} name=[]
        int ival;       //name=123
        float fval;     //name=123.456
        bool boolval;   //name=true
        struct{
            union{
                char* c_str;    //name="value"
                byte_t* binary; //name=<>
            };
            int size;
        };
    };
protected:
	xnode_type m_type;
	string_type m_name;
	string_type m_value;
	xnode* m_parent;
	xnode* m_subnode;
	xnode* m_prev;
	xnode* m_next;
public:
	xnode():m_type(node_unknow), m_name(), m_value(),
		m_parent(null), m_subnode(null), m_prev(null), m_next(null)
	{

    }
    ~xnode()
    {
        if(m_subnode){
            xnode* node = m_subnode;
            while(node){
                m_subnode = node->m_next;
                delete node;
                node = m_subnode;
            }
            m_subnode = null;
        }
        m_parent = m_prev = m_next = null;
    }

    xnode_type type()const
    {
        return m_type;
    }

    string_type name()const
    {
        return m_name;
    }

    string_type value()const
    {
        return m_value;
    }

    void set_value(const string_type& value)
    {
        m_value = value;
    }

    xnode* parent()const
    {
        return m_parent;
    }

    xnode* subnode()const
    {
        return m_subnode;
    }

    xnode* subnode(const string_type& name)const
	{
		xnode* node = this->subnode();
		while(node){
			if(node->name() == name){
				return node;
			}
			node = node->next();
        }
        return null;
    }

    xnode* previous()const
    {
        return m_prev;
    }

    xnode* next()const
    {
        return m_next;
    }

	xnode* previous_sibling()const
	{
		xnode* node = this->previous();
		while(node){
			if(node->name() == this->name()){
				return node;
			}
			node = node->previous();
        }
        return null;
    }

    xnode* next_sibling()const
    {
        xnode* node = this->next();
		while(node){
			if(node->name() == this->name()){
				return node;
			}
			node = node->next();
        }
        return null;
    }

	xnode* add(const string_type& name, const string_type& value = string_type(), xnode_type type = node_string)
	{
		if(!m_subnode){
			m_subnode = new xnode;
			m_subnode->m_type = type;
			m_subnode->m_name = name;
            m_subnode->m_value = value;
			m_subnode->m_parent = this;
			return m_subnode;
		}
		else{
			xnode* n = m_subnode;
			while(n->m_next){
				n = n->m_next;
			}
			n->m_next = new xnode;
			n->m_next->m_type = type;
			n->m_next->m_name = name;
            n->m_next->m_value = value;
			n->m_next->m_parent = this;
			n->m_next->m_prev = n;
			return n->m_next;
		}
	}

    int erase(const string_type& name)
    {
        xnode* node = this->subnode(name);
        if(node){
            return this->erase(node);
        }
        return -1;
    }

    int erase(xnode* node)
    {
        if(node->parent() == this){
            if(m_subnode == node){
                m_subnode = node->previous() ? node->previous() : node->next();
            }
            xnode *n = node->previous();
            if(n){
                n->m_next = node->next();
            }
            n = node->next();
            if(n){
                n->m_prev = node->previous();
            }
            delete node;
        }
        return -1;
    }

	bool to_bool(bool default_value = false)const
	{
		if(!m_value.empty())
		{
			return m_value.to_boolean();
		}
		else{
			return default_value;
		}
    }

	int to_int(int default_value = 0)const
	{
		if(!m_value.empty())
		{
			return m_value.to_int();
		}
		else{
			return default_value;
		}
	}

	float to_decimal(float default_value = 0.0f)const
	{
		if(!m_value.empty())
		{
			return m_value.to_decimal();
		}
		else{
			return default_value;
		}
	}
};

class xdocument
{
public:
	typedef cstring<char> string_type;
	xnode m_node;

    const static char assign       = '=';
    const static char object_begin = '{';
    const static char object_end   = '}';
    const static char array_begin  = '[';
    const static char array_end    = ']';
    const static char binary_begin = '<';
    const static char binary_end   = '>';
    const static char string_begin = '"';
    const static char string_end   = '"';
    const static char comma        = ',';
    const static char endline      = ';';
    const static char* comment_begin;
    const static char* comment_end;

public:
	xdocument():m_node()
	{
        m_node.m_type = node_object;
	}
	~xdocument()
	{
    }

    int open(const char* fname);
	int parse(const char* buf, size_t size = TEXT_MAX);
	int print(string_type& text)
	{
		return save(text, &m_node, 0);
	}
	int save(const string_type& filename);

	xnode* subnode()
	{
        return m_node.subnode();
	}

	xnode* subnode(const string_type& name)
	{
        return m_node.subnode(name);
	}

	xnode* add(const string_type& name, const string_type& value, xnode_type type)
	{
        return m_node.add(name, value, type);
    }

    int erase(const string_type& name)
    {
        return m_node.erase(name);
    }

    int erase(xnode* node)
    {
        return m_node.erase(node);
    }
private:
	inline void skip_space(const char* &begin, const char* end)
	{
		for( ; begin<end; ++begin)
		{
			if(begin[0] != ' ' && begin[0] != '\t' && begin[0] != '\r' && begin[0] != '\n'){
				break;
			}
        }
	}

	const char* read_string(string_type& str, const char* begin, const char* end);
	const char* read_binary(string_type& str, const char* begin, const char* end);
	string_type to_base64(const string_type& str);

	int save(string_type& text, xnode* node, int level);
	int save(FILE* f, xnode* node, int level);
};

int xdocument::open(const char* fname)
{
    FILE* f = fopen(fname, "r");
    size_t size = 0;
    char *buf = 0;
    if(!f)return -1;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = new char[size+1];
    size = fread(buf, 1, size, f);
    fclose(f);
    parse(buf, size);
    return 0;
}

const char* xdocument::read_string(xdocument::string_type& str, const char* begin, const char* end)
{
    for( ; begin<end; ++begin)
    {
        if(begin[0] == '\\'){
            switch(begin[1])
            {
            case 's':
                str.push_back(' ');
                break;
            case 'r':
                str.push_back('\r');
                break;
            case 'n':
                str.push_back('\n');
                break;
            case 't':
                str.push_back('\t');
                break;
            case 'p':
                str.push_back('\p');
                break;
            case '\\':
                str.push_back('\\');
                break;
            case '"':
                str.push_back('"');
                break;
            default:
                break;
            }
            ++begin;
        }
        else if(begin[0] == '"'){
            break;
        }
        else{
            str.push_back(begin[0]);
        }
    }
    return begin;
}

const char* xdocument::read_binary(xdocument::string_type& str, const char* begin, const char* end)
{
    for( ; begin<end; ++begin)
    {
        if(begin[0] != '>'){
            if(!isspace(begin[0])){//忽略掉空白
                str.push_back(begin[0]);
            }
        }
		else{
			char* buffer = new char[str.length()];
			size_t size = base64_decode(buffer, str.c_str(), str.length());
			str.assign(buffer, size);
            safe_delete(buffer);
            return begin;
        }
    }
    return begin;
}

xdocument::string_type xdocument::to_base64(const xdocument::string_type& str)
{
	char* buf = new char[str.length()*2];
	size_t size = base64_encode(buf, str.c_str(), str.length());
	cstring<char> s(buf, size);
	safe_delete(buf);
	return s;
}

int xdocument::parse(const char* text, size_t size)
{
    const char* begin = text;
	const char* end;
	string_type str;
	xnode* node = &m_node;
	const char* itr;

	if(size == TEXT_MAX){
		size = cslen(text);
	}
	end = text + cslen(text);

    skip_space(begin, end);
    for(; begin < end; )
    {
        switch(begin[0])
        {
        case '/'://comment
        {
            if(begin[1] == '/'){
                begin += 2;
                itr = std::find(begin, end-1, '\n')+1;
                str.assign(begin, itr);
                str.trim_right();
                begin = itr;
                node->add(string_type(), str, node_comment);
            }
            else if(begin[1] == '*'){
                begin += 2;
                for(itr = begin; itr<end-1; ++itr)
                {
                    if(itr[0] == '*' && itr[1] == '/')
                    {
                        break;
                    }
                }
                str.assign(begin, itr);
                if(itr[0] == '*'){
                    itr += 2;
                }
                else{
                    return -1;
                }
                begin = itr;
                node->add(string_type(), str, node_comments);
            }
            else{//parse error
                return -1;
            }
            str.clear();
            skip_space(begin, end);
            break;
        }
        case object_begin://push object
        {
            str.trim_right();
            node = node->add(str, string_type(), node_object);
            str.clear();
            ++begin;
            skip_space(begin, end);
            break;
        }
        case object_end://pop object
        {
            node = node->parent();
            skip_space(++begin, end);
            break;
        }
        case assign://push value
        {
            str.trim_right();
            node = node->add(str, string_type(), node_string);
            str.clear();
            skip_space(++begin, end);
            break;
        }
        case endline://pop value
        {
            if(node->type() == node_string || node->type() == node_binary){
                node->set_value(str);
                node = node->parent();
            }
            else if(node->type() == node_array)
			{

            }
            str.clear();
            skip_space(++begin, end);
            break;
        }
        case string_begin:
        {
            begin = read_string(str, ++begin, end);
            skip_space(++begin, end);
            break;
        }
        case array_begin://push array
        {
			node->m_type = node_array;
            skip_space(++begin, end);
            break;
        }
        case array_end://pop array
        {
            str.trim_right();
            if(!str.empty()){
                node->add(string_type(), str, node_string);
                str.clear();
			}
			node = node->parent();
			skip_space(++begin, end);
            break;
        }
        case comma://目前，只能存在数组里面
        {
            if(node->type() == node_array){
                node->add(string_type(), str, node_string);
                str.clear();
            }
            else{
                str.push_back(begin[0]);
            }
            skip_space(++begin, end);
            break;
        }
        case binary_begin://binary data
        {
            node->m_type = node_binary;
            str.clear();
            begin = read_binary(str, ++begin, end);
            skip_space(++begin, end);
            break;
        }
        default:
            str.push_back(begin[0]);
            ++begin;
            break;
        }
    }

	return 0;
}

int xdocument::save(xdocument::string_type& text, xnode* node, int level)
{
	if(!node)return -1;
	string_type space;
	xnode* n;
	const char* newline = "\r\n";

    space.resize(level, '\t');

	switch(node->type())
	{
	case node_object:
		text.append(space);
		text.append(node->name());
		text.append(newline);
		text.append(space);
		text.append(object_begin);
		text.append(newline);
		save(text, node->subnode(), level+1);//打印subnode
		text.append(space);
		text.append(object_end);
		text.append(newline);
		break;
	case node_comment:
		text.append(space);
		text.append("//");
		text.append(node->value());
		text.append(newline);
		break;
    case node_comments:
		text.append(space);
		text.append("/*");
		text.append(node->value());
		text.append("*/");
		text.append(newline);
		break;
	case node_array:
		text.append(space);
		text.append(node->name());
		text.append(assign);
        text.append(array_begin);
		n = node->subnode();
		while(n){
			text.append('\"');
			text.append(to_escape(n->value()));
			text.append('\"');
			if(n->next()){
				text.append(',');
			}
			n = n->next();
		}
		text.append(array_end);
        text.append(endline);
		text.append(newline);
		break;
	case node_binary:
		text.append(space);
		text.append(node->name());
        text.append(assign);
		text.append(binary_begin);
		text.append(to_base64(node->value()));
		text.append(binary_end);
        text.append(endline);
		text.append(newline);
		break;
	case node_string:
		text.append(space);
		text.append(node->name());
		if(node->value().empty()){
			text.append(assign);
            text.append(endline);
			text.append(newline);
		}
		else{
            text.append(assign);
            text.append(string_begin);
			text.append(to_escape(node->value()));
			text.append(string_end);
            text.append(endline);
			text.append(newline);
        }
		break;
	default:
		break;
	}
	save(text, node->next(), level);
	return text.length();
}

int xdocument::save(const string_type& filename)
{
	FILE* f = fopen(filename.c_str(), "wb");
	this->save(f, &m_node, 0);
	fclose(f);
	return 0;
}

void writefile(const cstring<char>& s, FILE* f)
{
	if(!s.empty())
	//fputs(s.c_str(), f);
	fwrite(s.c_str(), s.length(), 1, f);
}

int xdocument::save(FILE* f, xnode* node, int level)
{
	if(!node)return -1;
	string_type space;
	xnode* n;
	const char* newline = "";

    space.resize(0, '\t');

	switch(node->type())
	{
	case node_object:
		fputs(space.c_str(), f);
		writefile(node->name(), f);
		writefile(newline, f);
		writefile(space, f);
		writefile(object_begin, f);
		writefile(newline, f);
		if(node->subnode())save(f, node->subnode(), level+1);//打印subnode
		writefile(space, f);
		writefile(object_end, f);
		writefile(newline, f);
		break;
	case node_comment:
		writefile(space, f);
		writefile("//", f);
		writefile(node->value(), f);
		writefile(newline, f);
		break;
    case node_comments:
		writefile(space, f);
		writefile("/*", f);
		writefile(node->value(), f);
		writefile("*/", f);
		writefile(newline, f);
		break;
	case node_array:
		writefile(space, f);
		writefile(node->name(), f);
		writefile(assign, f);
        writefile(array_begin, f);
		n = node->subnode();
		while(n){
			writefile('\"', f);
			writefile(to_escape(n->value()), f);
			writefile('\"', f);
			if(n->next()){
				writefile(',', f);
			}
			n = n->next();
		}
		writefile(array_end, f);
        writefile(endline, f);
		writefile(newline, f);
		break;
	case node_binary:
		writefile(space, f);
		writefile(node->name(), f);
        writefile(assign, f);
		writefile(binary_begin, f);
		writefile(to_base64(node->value()), f);
		writefile(binary_end, f);
        writefile(endline, f);
		writefile(newline, f);
		break;
	case node_string:
		writefile(space, f);
		writefile(node->name(), f);
		if(node->value().empty()){
			writefile(assign, f);
            writefile(endline, f);
			writefile(newline, f);
		}
		else{
            writefile(assign, f);
            writefile(string_begin, f);
			writefile(to_escape(node->value()), f);
			writefile(string_end, f);
            writefile(endline, f);
			writefile(newline, f);
        }
		break;
	default:
		break;
	}
	if(node->next())save(f, node->next(), level);
	return 0;
}

_CGL_END

#endif //PSET_HPP_20170104104641

