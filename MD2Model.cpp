#include <string>
#include <iostream>
#include <fstream>
#include "MD2Model.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//This code is referenced and some of it copy and pasted from David Henry - tfc_duke@hotmail.com md2 loader tutorial

/********************************
*animList
*hardcoded list of data
*used for animation of md2 models
*has the start frame end frame and FPS
********************************/
anim_t animlist[ 21 ] =
{
    // first, last, fps

    {   0,  39,  9 },	// STAND
    {  40,  45, 10 },	// RUN
    {  46,  53, 10 },	// ATTACK
    {  54,  57,  7 },	// PAIN_A
    {  58,  61,  7 },	// PAIN_B
    {  62,  65,  7 },	// PAIN_C
    {  66,  71,  7 },	// JUMP
    {  72,  83,  7 },	// FLIP
    {  84,  94,  7 },	// SALUTE
    {  95, 111, 10 },	// FALLBACK
    { 112, 122,  7 },	// WAVE
    { 123, 134,  6 },	// POINT
    { 135, 153, 10 },	// CROUCH_STAND
    { 154, 159,  7 },	// CROUCH_WALK
    { 160, 168, 10 },	// CROUCH_ATTACK
    { 196, 172,  7 },	// CROUCH_PAIN
    { 173, 177,  5 },	// CROUCH_DEATH
    { 178, 183,  7 },	// DEATH_FALLBACK
    { 184, 189,  7 },	// DEATH_FALLFORWARD
    { 190, 197,  7 },	// DEATH_FALLBACKSLOW
    { 198, 198,  5 },	// BOOM
};

/********************************
*Default Constructor MD2Model
*Initialises MD2Model data structures
*to sane defaults
********************************/
MD2Model::MD2Model()
{
    vertices_m = 0;
    glCmds_m = 0;
    lightNormals_m = 0;

    numFrames = 0;
    numVerts = 0;
    numGlCmds = 0;

    texId_m = 0;
    scale_m = 1.0f;
    setAnimation(STAND);

    position[0] = 0.0f;
    position[1] = 0.0f;
    position[2] = 0.0f;

    rotation[0] = 0.0f;
    rotation[1] = 0.0f;
    rotation[2] = 0.0f;

    velocity[0] = 0.0f;
    velocity[1] = 0.0f;
    velocity[2] = 0.0f;

    time = 0;
}

/********************************
*Deconstructor MD2Model
*Deletes the heap data structures before
*exiting
********************************/
MD2Model::~MD2Model()
{
    delete[] vertices_m;
    delete[] glCmds_m;
    delete[] lightNormals_m;

}

MD2Model::MD2Model(std::string modelPath)
{

}

/********************************
*draw
*Moves to the models required
*position and rotation then calls the
*drawModel method with the last updated time
*to draw the model
********************************/
void MD2Model::draw()
{
    glPushMatrix();
        //glLoadIdentity();
        glTranslatef(position[0], position[1], position[2]);
        glRotatef(rotation[0], 1, 0, 0);
        glRotatef(rotation[1], 0, 1, 0);
        glRotatef(rotation[2], 0, 0, 1);
            drawModel(time);
    glPopMatrix();
}

/********************************
*update
*Sets the current time to the given time
*Then moves the models position depending on
*the models velocity
********************************/
void MD2Model::update(float time)
{
    this->time = time;
    position[0] += velocity[0];
    position[1] += velocity[1];
    position[2] += velocity[2];

}

/********************************
*scaleModel
*Changes the size of the model
*used when creating the vertices positions
********************************/
void MD2Model::scaleModel(float sFactor)
{
    scale_m = sFactor;
}

/********************************
*loadModel
*Opens the given file and reads the MD2Header
*then from the header details reads in the
*frame data, opengl commands and decompresses the
*vertices data from the file
********************************/
bool MD2Model::loadModel(std::string filename)
{
    std::ifstream file;
    bool retVal = false;
    struct md2header_s header;
    char* buffer;
    frame_t* frame;
    vec3_t* ptrVerts;
    int*    ptrnormals;

    file.open( filename.c_str(), std::ios::in | std::ios::binary);

    if( file.fail())
    {
        printf("Model failed to load\n");
    }
    else
    {

        file.read((char*)&header, sizeof(header));

        if((header.ident == MD2IDENT) && (header.version == MD2VERSION))
        {
            printf("Header IDENT %d, matches %d and version matches\n", header.ident, MD2IDENT);

            numFrames = header.numFrames;
            numVerts = header.numVerts;
            numGlCmds = header.numGlCmds;

            vertices_m = new vec3_t[numVerts * numFrames];
            glCmds_m = new int[numGlCmds];
            lightNormals_m = new int[numVerts * numFrames];

            buffer = new char[numFrames * header.frameSize];

            //read the frame data into the buffers
            file.seekg( header.framesOfs, std::ios::beg);
            file.read((char*)buffer, numFrames * header.frameSize);

            //opengl cmds
            file.seekg(header.glCmdsOfs, std::ios::beg);
            file.read((char*)glCmds_m, numGlCmds * sizeof(int));

            for(int j = 0; j < numFrames; j++)
            {
                frame = (frame_t*)&buffer[header.frameSize * j];
                ptrVerts = &vertices_m[numVerts * j];
                ptrnormals = &lightNormals_m[numVerts * j];

                for(int i = 0; i < numVerts; i++)
                {
                    ptrVerts[i][0] = (frame->verts[i].vert[0] * frame->scale[0]) + frame->translate[0];
                    ptrVerts[i][1] = (frame->verts[i].vert[1] * frame->scale[1]) + frame->translate[1];
                    ptrVerts[i][2] = (frame->verts[i].vert[2] * frame->scale[2]) + frame->translate[2];

                    ptrnormals[i] = frame->verts[i].normalIndex;
                }
            }

            delete[] buffer;
            file.close();
            retVal = true;
        }
        else
        {
            printf("Header does not match failed to load\n");

        }

    }

    return retVal;
}

/********************************
*loadTexture
*Helper function that loads the given
*image as a texture
*Checks the type of image RGB or RGBA
*and the endiness
*then sets the parameters and loads the
*texture into the opengl state
********************************/
unsigned int MD2Model::loadTexture(std::string textureName)
{
    /*unsigned int texture;
    GLint nOfColors;
    GLenum texture_format;
    SDL_Surface* temp = NULL;
    temp = IMG_Load( textureName.c_str());
    SDL_Surface* sprite = NULL;

    if( temp != NULL )
    {
        std::cout<<"Loaded "<<"bsic.png"<<std::endl;

        sprite = SDL_DisplayFormatAlpha( temp );

        if( sprite == NULL )
        {
            printf( "%s could not be loaded", "./basic.png");
        }

        SDL_SetAlpha( sprite, 0, 0 );

        SDL_FreeSurface( temp );

        if( ( sprite->w & ( sprite->w - 1 ) ) !=0 )
        {
            printf( "Warning: sprite width is not a power of 2\n");
        }

        if( ( sprite->h & ( sprite->h - 1 ) ) !=0 )
        {
            printf( "Warning: sprite height is not a power of 2\n");
        }

        nOfColors = sprite->format->BytesPerPixel;
        if( nOfColors == 4 )
        {
            if( sprite->format->Rmask == 0x000000ff)
                texture_format=GL_RGBA;
            else
                texture_format=GL_BGRA;
        }
        else if( nOfColors == 3 )
        {
            if( sprite->format->Rmask == 0x000000ff)
                texture_format=GL_RGB;
            else
                texture_format=GL_BGR;
        }
        else
        {
            printf( "Warning: the image is not true color this will break\n");
        }
    }

    glGenTextures( 1, &texture );

    glBindTexture( GL_TEXTURE_2D, texture);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, sprite->w, sprite->h, 0, texture_format, GL_UNSIGNED_BYTE, sprite->pixels );

    SDL_FreeSurface(sprite);

    return texture;*/

}

/********************************
*loadSkin
*sets the models texture id to
*the one returned by loadTexture
*with the given image path
********************************/
bool MD2Model::loadSkin(std::string filename)
{
    texId_m = loadTexture(filename);

    return (true);
}

/********************************
*drawFrame
*sets the animation frame to the
*specified one and then draws the models at time 1
*Used to draw the model at a specific frame
********************************/
void MD2Model::drawFrame(int frame)
{
    anim_m.startFrame = frame;
    anim_m.endFrame = frame;
    anim_m.nextFrame = frame;
    anim_m.fps = 1;
    anim_m.type = -1;

    drawModel(1.0);
}

/********************************
*renderFrame
*Calls interpolate function to moves the
*vertices to the correct place between the key frames
*Binds the texture
*then draws the vertics to the screen
********************************/
void MD2Model::renderFrame()
{
    int* ptrglcmds = glCmds_m;
    vec3_t vertList[MAX_MD2_VERTS];

    glPushAttrib(GL_POLYGON_BIT);
    glFrontFace(GL_CW);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //processLighting()

    interpolate(vertList);

    glBindTexture(GL_TEXTURE_2D, texId_m);

    while( int i = *(ptrglcmds++))
    {
        if(i < 0)
        {
            glBegin(GL_TRIANGLE_FAN);
            i = -i;
        }
        else
        {
            glBegin(GL_TRIANGLE_STRIP);
        }

        for(    ; i > 0; i--, ptrglcmds+= 3)
        {
            glTexCoord2f( ((float*)ptrglcmds)[0], ((float*)ptrglcmds)[1]);

            glNormal3fv( anorms[ lightNormals_m[ ptrglcmds[2] ] ]);

            glVertex3fv(vertList[ptrglcmds[2]]);
        }

        glEnd();
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_CULL_FACE);
    glPopAttrib();
}

/********************************
*animate
*Updates the current frame of animation
*depending on how much time has passed since
*the last frame was animated
*also sets the interpolation value
*used to interpolate between key frames
********************************/
void MD2Model::animate(float time)
{
    anim_m.currTime = time;

    if(anim_m.currTime - anim_m.oldTime > (1.0 / anim_m.fps))
    {
        anim_m.currFrame = anim_m.nextFrame;
        anim_m.nextFrame++;

        if(anim_m.nextFrame > anim_m.endFrame)
        {
            anim_m.nextFrame = anim_m.startFrame;
        }

        anim_m.oldTime = anim_m.currTime;
    }

    if(anim_m.currFrame > (numFrames - 1))
    {
        anim_m.currFrame = 0;
    }

    if(anim_m.nextFrame > (numFrames - 1))
    {
        anim_m.nextFrame = 0;
    }

    anim_m.interpol = anim_m.fps * (anim_m.currTime - anim_m.oldTime);
}

/********************************
*drawModel
*Calls animate to change to the correct frame
*then uses renderframe to render the vertices
*Has to rotate the model to get it to sit
*correctly within opengl
********************************/
void MD2Model::drawModel(float time)
{
    if(time > 0.0f)
    {
        animate(time);
    }

    glPushMatrix();
        glRotatef(-90.0, 1, 0, 0);
        glRotatef(-90.0, 0, 0, 1);

        renderFrame();
    glPopMatrix();
}

/********************************
*interpolate
*Gets the current frame and the next frame
*and interpolates the vertices new position
*depending on how much time has occureed between
* the two frames using linear interpolation
********************************/
void MD2Model::interpolate(vec3_t* vertList)
{
    vec3_t* currFrame = &vertices_m[numVerts * anim_m.currFrame];
    vec3_t* nextFrame = &vertices_m[numVerts * anim_m.nextFrame];

    for(int i = 0; i < numVerts; i++)
    {
        vertList[i][0] = (currFrame[i][0] + anim_m.interpol * (nextFrame[i][0] - currFrame[i][0])) * scale_m;
        vertList[i][1] = (currFrame[i][1] + anim_m.interpol * (nextFrame[i][1] - currFrame[i][1])) * scale_m;
        vertList[i][2] = (currFrame[i][2] + anim_m.interpol * (nextFrame[i][2] - currFrame[i][2])) * scale_m;
    }
}

/********************************
*setAnimation
*Updates the animation state to
*the specified animation type
********************************/
void MD2Model::setAnimation(int type)
{
    if(anim_m.type != type)
    {
        anim_m.startFrame = animlist[type].firstFrame;
        anim_m.endFrame = animlist[type].lastFrame;
        anim_m.fps = animlist[type].fps;

        anim_m.nextFrame = animlist[type].firstFrame + 1;
        anim_m.type = type;
    }
}

/********************************
*setPosition
*Sets the position the model should be at
*when it translates before it is drawn
********************************/
void MD2Model::setPosition(float x, float y, float z)
{
        position[0] = x;
        position[1] = y;
        position[2] = z;

}

/********************************
*setRotation
*Sets the amount the model should be rotated
*on each axis before being drawn to the screen
********************************/
void MD2Model::setRotation(float x, float y, float z)
{
        rotation[0] = x;
        rotation[1] = y;
        rotation[2] = z;

}

/********************************
*setVelocity
*Sets how fast and in what directions
*the Model should be moving
*Used to update the models position after each frame
********************************/
void MD2Model::setVelocity(float x, float y, float z)
{
        velocity[0] = x;
        velocity[1] = y;
        velocity[2] = z;

}
