// This file is part of the OGRE project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at https://www.ogre3d.org/licensing.
// SPDX-License-Identifier: MIT

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreCameraMan.h"

class MyTestApp : public OgreBites::ApplicationContext, public OgreBites::InputListener
{
public:
    MyTestApp();
    void setup();
    bool keyPressed(const OgreBites::KeyboardEvent& evt);
};

//! [constructor]
MyTestApp::MyTestApp() : OgreBites::ApplicationContext("OgreTutorialApp")
{
}
//! [constructor]

//! [key_handler]
bool MyTestApp::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
        closeApp();
    }
    return true;
}
//! [key_handler]

//! [setup]
void MyTestApp::setup(void)
{
    // do not forget to call the base first
    OgreBites::ApplicationContext::setup();

    // register for input events
    addInputListener(this);

    // get a pointer to the already created root
    Ogre::Root* root = getRoot();
    Ogre::SceneManager* scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    // without light we would just get a black screen    
    Ogre::Light* light = scnMgr->createLight("MainLight");
    Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 10, 15);
    lightNode->attachObject(light);

    //add ambient light
    scnMgr->setAmbientLight(Ogre::ColourValue::Red);

    // also need to tell where we are. Set position of node holding the camera
    Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(0, 0, 80);
    camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

    // create the camera
    Ogre::Camera* cam = scnMgr->createCamera("myCam");
    cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);

    //add camera man
    OgreBites::CameraStyle cameraStyle = OgreBites::CameraStyle::CS_FREELOOK;
    OgreBites::CameraMan* cameraMan = new OgreBites::CameraMan(camNode);
    cameraMan->setStyle(cameraStyle);
    cameraMan->setTopSpeed(60);
    addInputListener(cameraMan);

    // and tell it to render into the main window
    auto viewport = getRenderWindow()->addViewport(cam);
    viewport->setClearEveryFrame(true);

    // load resources
    std::string path = "Meshes";
    std::string resourcesGroupName = "ExampleGroup";
    Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(path, "FileSystem", resourcesGroupName);
    Ogre::ResourceGroupManager::getSingletonPtr()->initialiseResourceGroup(resourcesGroupName);
    Ogre::ResourceGroupManager::getSingletonPtr()->loadResourceGroup(resourcesGroupName);


    // adding elements to scene graph
    Ogre::Entity* sinbad1 = scnMgr->createEntity("SinbadExample.mesh");
    Ogre::SceneNode* node1 = scnMgr->getRootSceneNode()->createChildSceneNode();
    node1->translate(Ogre::Vector3(5, 15, 0));
    node1->attachObject(sinbad1);
    

    Ogre::Entity* sinbad2 = scnMgr->createEntity("SinbadExample.mesh");
    Ogre::SceneNode* node2 = scnMgr->getRootSceneNode()->createChildSceneNode();
    node2->scale(Ogre::Vector3(5, 5, 5));
    node2->attachObject(sinbad2);
    


    //add floor
    Ogre::Plane plane;
    plane.normal = Ogre::Vector3::UNIT_Y;
    plane.d = 0;

    Ogre::MeshManager::getSingleton().createPlane(
        "floor",
        resourcesGroupName,
        plane,
        450.0, 450.0,
        10, 10, true, 1,
        50.0, 50.0,
        Ogre::Vector3::UNIT_Z);

    Ogre::Entity* plane_entity = scnMgr->createEntity("Plane_Grid", "floor");
    plane_entity->setMaterialName("Sinbad/Body",resourcesGroupName);

    Ogre::SceneNode* plane_node = scnMgr->getRootSceneNode()->createChildSceneNode("Plane_Node");
    plane_node->translate(Ogre::Vector3(0, -25, 0));
    plane_node->attachObject(plane_entity);
}
//! [setup]

//! [main]
int main(int argc, char* argv[])
{
    MyTestApp app;
    app.initApp();
    //app.getRoot()->startRendering();
    while (!app.getRenderWindow()->isClosed())
    {
        app.getRoot()->renderOneFrame();
    }
    app.closeApp();
    return 0;
}
//! [main]
