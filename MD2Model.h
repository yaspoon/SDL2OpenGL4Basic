#ifndef MD2MODEL_H
#define MD2MODEL_H

#include <SDL2/SDL_opengl.h>

//This code is referenced and some of it copy and pasted from David Henry - tfc_duke@hotmail.com md2 loader tutorial

#define MAX_MD2_VERTS			2048
#define NUMVERTEXNORMALS		162

/********************************
*md2Header_s
*Represents a md2model
*Read from the md2 model file and used
*to read the other data from the file
*Holds the important data to use a md2 model
********************************/
struct md2header_s
{
    int ident;  //Identification should be IDP2
    int version;    //model version should be 8

    int skinWidth;  //texture width
    int skinHeight; //texture height
    int frameSize;  //size of each animation frame

    int numSkins;   //Number of different
    int numVerts;    //number of vertices
    int numTexCoords; //number of texture coords
    int numTris;     //number triangles
    int numGlCmds;     //number of opengl commands used for triangles
    int numFrames;   //total number of frames

    int skinsOfs;   //offset of skin data
    int texCoordOfs; //offset of texture coords
    int triangleOfs; //offset of triangles
    int framesOfs;  //frames offset
    int glCmdsOfs;  //opengl cmds offset
    int endOfs;     //offset to the enf of file
};

//simple array used for vectors [x,y,z]
typedef GLfloat vec3_t[3];

static vec3_t	anorms[ NUMVERTEXNORMALS ] = {
    #include	"anorms.h"
};

//Used when loading vertexs from file
typedef struct
{
    unsigned char vert[3];      //vertexs x,y,z compressed using some cool stuff
    unsigned char normalIndex; //index into normal array for this vertex
} vertex_t;

//Holds a texture coordinate and is compressed
typedef struct
{
    short   s;  //im assuming these represent s and t tangents
    short   t;
} texCoord_t;

//Keyframes uses scale and translate to uncompress each vertex for the frame
typedef struct
{
    vec3_t scale;       //scale the vert then translate it to its correct place
    vec3_t translate;
    char name[16];      //frame name
    vertex_t verts[1];     //holds the first vertex of the frame in an array frame.verts[i]
}frame_t;

//holds a current animations first last and FPS
typedef struct
{
    int firstFrame;
    int lastFrame;
    int fps;
}anim_t;

//current state the models animation is in
typedef struct
{
    int startFrame;
    int endFrame;
    int fps;

    float currTime;
    float oldTime;
    float interpol;

    int type;

    int currFrame;
    int nextFrame;

}animState_t;

// animation list
// this was directly copy pasted from David Henry - tfc_duke@hotmail.com
typedef enum {
	STAND,
	RUN,
	ATTACK,
	PAIN_A,
	PAIN_B,
	PAIN_C,
	JUMP,
	FLIP,
	SALUTE,
	FALLBACK,
	WAVE,
	POINT,
	CROUCH_STAND,
	CROUCH_WALK,
	CROUCH_ATTACK,
	CROUCH_PAIN,
	CROUCH_DEATH,
	DEATH_FALLBACK,
	DEATH_FALLFORWARD,
	DEATH_FALLBACKSLOW,
	BOOM,

	MAX_ANIMATIONS

} animType_t;

/********************************
*Class MD2Model
*Encampsulates a md2 model withing a object
*Used to load, draw and animate a md2Model
********************************/
class MD2Model
{
    private:
        static const int MD2IDENT = 'I' + ('D'<<8) + ('P'<<16) + ('2'<<24);
        static const int MD2VERSION = 8;

        float time;

        int numFrames;
        int numVerts;
        int numGlCmds;

        vec3_t position;
        vec3_t rotation;
        vec3_t velocity;

        vec3_t* vertices_m;
        int* glCmds_m;
        int* lightNormals_m;

        unsigned int texId_m;
        animState_t anim_m;
        float scale_m;

        void animate(float time);
        void processLighting(void);
        void interpolate(vec3_t* vertList);
        void renderFrame(void);
        unsigned int loadTexture(std::string textureName);

    public:
        MD2Model();
        virtual ~MD2Model();
        MD2Model( std::string modelName);

        void draw();
        void update(float time);

        bool loadModel(std::string filename);
        bool loadSkin(std::string filename);

        void drawModel(float time);
        void drawFrame(int frame);

        void setAnimation(int type);
        void scaleModel(float sFactor);

        void setPosition(float x, float y, float z);
        void setRotation(float x, float y, float z);
        void setVelocity(float x, float y, float z);

};
#endif
