
class stlmodel
{
public:
    std::vector<vertex> vertices;

    void open(const char* filename, double scale = 1.0)
    {
        std::ifstream f(filename);
        if(!f.is_open()){
            qDebug("model load error: %s", filename);
            return ;
        }

        vertices.clear();

        std::string line;
        std::string str;

        vertex v;
        v.color = vec4(1.0);
        while(!f.eof()){
            std::getline(f, line);
            std::stringstream stm(line);

            stm>>str;
            if(str == "solid"){
                continue;
            }
            if(str == "facet"){
                stm>>str;
                if(str == "normal"){
                    stm >> v.normal.x >> v.normal.y >> v.normal.z;
                }
            }
            if(str == "outer"){
                continue;
            }
            if(str == "vertex"){
                stm >> v.position.x >> v.position.y >> v.position.z;
                v.position *= scale;
                vertices.push_back(v);
            }
        }
    }
};
