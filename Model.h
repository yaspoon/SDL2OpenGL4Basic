#ifndef MODEL_H
#define MODEL_H

struct pmMaterial
{
        size_t length; //Length of path string
        char *path;
        size_t next; //Offset to next material
};

struct pmHeader
{
        int vCount; //Vertex Count
        int tCount; //Texture Coord count
        int mCount; //Material count

        size_t  vOffset; //Byte offset of vertex we could just calculate this on the fly but hdd space is cheap right?
        size_t tOffset; //ditto
        size_t mOffset; //Where the material stuff starts

        //char *data - data starts directly after the header.
};

class Model
{
private:

public:
        Model(std::string path); //Load model from binary at path location
};

#endif // MODEL_H
