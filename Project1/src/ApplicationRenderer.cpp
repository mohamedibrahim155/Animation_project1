#include"ApplicationRenderer.h"


ApplicationRenderer::ApplicationRenderer()
{
}

ApplicationRenderer::~ApplicationRenderer()
{
}



void ApplicationRenderer::WindowInitialize(int width, int height,  std::string windowName)
{
    windowWidth = width;
    WindowHeight = height;
    lastX = windowWidth / 2.0f;
    lastY= WindowHeight / 2.0f;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* w, int x, int y)
        {
            static_cast<ApplicationRenderer*>(glfwGetWindowUserPointer(w))->SetViewPort(w, x, y);
        });

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            static_cast<ApplicationRenderer*>(glfwGetWindowUserPointer(window))->KeyCallBack(window, key, scancode, action, mods);
        });


    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xposIn, double yposIn)
        {
            static_cast<ApplicationRenderer*>(glfwGetWindowUserPointer(window))->MouseCallBack(window, xposIn, yposIn);
        });

    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            static_cast<ApplicationRenderer*>(glfwGetWindowUserPointer(window))->MouseScroll(window, xoffset, yoffset);
        });
   
   

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.FontGlobalScale = 2.0f;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    //Init GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }


   

  
    defaultShader = new Shader("Shaders/DefaultShader_Vertex.vert", "Shaders/DefaultShader_Fragment.frag");
    SolidColorShader = new Shader("Shaders/SolidColor_Vertex.vert", "Shaders/SolidColor_Fragment.frag", SOLID);
    StencilShader = new Shader("Shaders/StencilOutline.vert", "Shaders/StencilOutline.frag", OPAQUE);
   
    alphaBlendShader = new Shader("Shaders/DefaultShader_Vertex.vert", "Shaders/DefaultShader_Fragment.frag", ALPHA_BLEND);
    alphaBlendShader->blendMode = ALPHA_BLEND;

    alphaCutoutShader = new Shader("Shaders/DefaultShader_Vertex.vert", "Shaders/DefaultShader_Fragment.frag", ALPHA_CUTOUT);
    alphaCutoutShader->blendMode = ALPHA_CUTOUT;

    SkyboxShader = new Shader("Shaders/SkyboxShader.vert", "Shaders/SkyboxShader.frag");
    SkyboxShader->modelUniform = false;

    Model* skyBoxMod = new Model("Models/DefaultCube/DefaultCube.fbx",false);

    skyBoxMod->meshes[0]->meshMaterial = new SkyboxMaterial();

    SkyboxMaterial* _skyBoxMaterial = skyBoxMod->meshes[0]->meshMaterial->skyboxMaterial();

    std::vector<std::string> faces
    {
       ("Textures/skybox/right.jpg"),
       ("Textures/skybox/left.jpg"),
       ("Textures/skybox/top.jpg"),
       ("Textures/skybox/bottom.jpg"),
       ("Textures/skybox/front.jpg"),
       ("Textures/skybox/back.jpg")
    };
    _skyBoxMaterial->skyBoxTexture->LoadTexture(faces);

    render.SkyBoxModel = skyBoxMod;
   // render.AddModelsAndShader(render.SkyBoxModel, SkyboxShader);

    //ScrollShader = new Shader("Shaders/ScrollTexture.vert", "Shaders/ScrollTexture.frag");
    render.AssignStencilShader(StencilShader);


    camera.transform.position = glm::vec3(0, 0, - 1.0f);
}



void ApplicationRenderer::Start()
{
 //   GLCALL(glEnable(GL_DEPTH_TEST));
    GLCALL(glDepthFunc(GL_LESS));
    GLCALL(glEnable(GL_STENCIL_TEST));
    GLCALL(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
    GLCALL(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
    GLCALL(glEnable(GL_BLEND));
    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


   /* skybox = new Skybox(); 
    
    skybox->AssignSkyboxShader(SkyboxShader);
    skybox->SkyboxPrerender();*/
    

    render.AssignCamera(&camera);

    Model* Sphere = new Model((char*)"Models/DefaultSphere/Sphere_1_unit_Radius.ply", true);
   // Model* Sphere = new Model();

   // Model* scroll = new Model((char*)"Models/Scroll/scroll.obj", true);
   // scroll->transform.position.y -= 5;
   // scroll->meshes[0]->TextureScrolling(true);


     Model* Pokeball = new Model((char*)"Models/Pokeball/pokeball.obj");

    Model* floor = new Model((char*)"Models/Floor/Floor.fbx");
    floor->transform.SetRotation(glm::vec3(90, 0, 0));
    floor->transform.SetPosition(glm::vec3(0, -2, 0));
   
    Model* floor2 = new Model(*floor);
    floor2->transform.SetRotation(glm::vec3(90, 0, 0));
    floor2->transform.SetPosition(glm::vec3(0, 2, 0));
   
   
    Model* floor3 = new Model(*floor);
   
    floor3->transform.SetPosition(glm::vec3(-2, 0, 0));
    Model* floor4 = new Model(*floor);
    floor4->transform.SetPosition(glm::vec3(2, 0, 0));
    floor4->meshes[0]->meshMaterial->material()->useMaskTexture = false;
    floor4->meshes[0]->meshMaterial->material()->SetBaseColor(glm::vec4(1, 1, 1, 0.5f));

   //  Model* Pokeball2 = new Model((char*)"Models/Pokeball/pokeball.obj", true);


    // Model* Grass = new Model((char*)"Models/Grass/Grass.obj", true,true, true);
    
   /*  Model* Window = new Model();
     Window->alphaMask = new Texture();
     
     Window->alphaMask->LoadTexture("Models/Window/WindowAlphaMask.png","opacity_Texture");
     Window->loadModel("Models/Window/Window.obj");
     Window->isTransparant = true;
     Window->isCutOut = false;

     Model* Window2 = new Model();
     Window2->alphaMask = new Texture();
     Window2->isTransparant = true;
     Window2->isCutOut = false;
     Window2->alphaMask->LoadTexture("Models/Window/WindowAlphaMask.png", "opacity_Texture");
     Window2->loadModel((char*)"Models/Window/Window.obj");*/
   
   
     


     Sphere->transform.position.x += 2;
     Pokeball->transform.position.x -= 2;
    


     Model* dir = new Model("Models/DefaultSphere/Sphere_1_unit_Radius.ply",false);
     dir->transform.SetScale(glm::vec3(0.5f));
    // Model* spotlight = new Model(*Sphere);
     //spotlight->transform.SetPosition(glm::vec3(-2.0f, 0.0f, -3.0f));

     Light directionLight;
     directionLight.lightType = LightType::DIRECTION_LIGHT;
     directionLight.lightModel = dir;
     directionLight.ambient =  glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
   //  directionLight.diffuse =  glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
    // directionLight.specular = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
     directionLight.color = glm::vec4(1, 1, 1, 1.0f);
     directionLight.linear = 1;
     directionLight.constant = 1;
     directionLight.quadratic = 0.01f;
     directionLight.cutOffAngle = 11;
     directionLight.outerCutOffAngle = 12;
     dir->transform.SetRotation(glm::vec3(0, 0, 0));
     dir->transform.SetPosition(glm::vec3(0, 0, 2));

    
     Model* plant = new Model("Models/Plant.fbm/Plant.fbx");
     Texture* plantAlphaTexture = new Texture();
    // plantAlphaTexture->LoadTexture("Models/Plant.fbm/Plant.fbm/Leaf_Front_1_2_Opacity.png", "opacity_Texture");
   //  plantAlphaTexture->LoadTexture("Models/Plant.fbm/Plant.fbm/Leaf_Front_1_2.png", "opacity_Texture");
      //   plant->meshes[0]->meshMaterial->material()->alphaTexture = plantAlphaTexture;
 //   plant->meshes[0]->meshMaterial->material()->useMaskTexture = fa;


    // render.AddModelAndShader(Pokeball, defaultShader);
   // render.AddModelAndShader(plant, alphaCutoutShader);
   // render.AddModelAndShader(floor, defaultShader);
   // render.AddModelAndShader(floor2, defaultShader);
   // render.AddModelAndShader(floor3, defaultShader);
   // render.AddModelAndShader(floor4, alphaBlendShader);
  



     
  //   render.selectedModel = Sphere;

     render.AddModelAndShader(dir,SolidColorShader);


     //LightRenderer
     lightManager.AddNewLight(directionLight);
   //  lightManager.AddNewLight(spot);
   //  lightManager.SetUniforms(defaultShader->ID);
   //  PhysicsObject* SpherePhyiscs = new PhysicsObject(Sphere);
   //  SpherePhyiscs->Initialize(false, true, DYNAMIC);

   //  PhysicsEngine.AddPhysicsObjects(SpherePhyiscs);
     


#pragma region Animation StartUps
     dir->entityID = "DirectionLight";
     dir->InitializeAnimation();

    // Animation* directionLightAnimation = dir->animation;

     dir->animation->AddPositionKeyFrame(glm::vec3(5, 0, 0), 0);
     dir->animation->AddPositionKeyFrame(glm::vec3(5, 2, 0), 1);
     dir->animation->AddPositionKeyFrame(glm::vec3(5, 5, 0), 2);
     dir->animation->AddPositionKeyFrame(glm::vec3(2, 5, 0), 3);

     dir->animation->AddColoreKeyFrame(glm::vec3(1, 1, 1), 0, EasingType::sineEaseIn);
     dir->animation->AddColoreKeyFrame(glm::vec3(0, 1, 0), 1, EasingType::sineEaseInOut);
     dir->animation->AddColoreKeyFrame(glm::vec3(0, 0, 1), 5, EasingType::sineEaseOut);


     Model* SecondModel = new Model(*plant);
     render.AddModelAndShader(SecondModel, defaultShader);

     SecondModel->entityID = "SecondModel";
     SecondModel->InitializeAnimation();

     SecondModel->animation->AddPositionKeyFrame(glm::vec3(-5, 0, 0), 0);
     SecondModel->animation->AddPositionKeyFrame(glm::vec3(-5, 2, 0), 1, EasingType::sineEaseIn);
     SecondModel->animation->AddPositionKeyFrame(glm::vec3(-5, 5, 0), 2, EasingType::sineEaseInOut);
     SecondModel->animation->AddPositionKeyFrame(glm::vec3(-2, 5, 0), 3, EasingType::sineEaseOut);

     SecondModel->animation->AddRotationKeyFrame(glm::quat(1.0f, 0.f, 0.f, 0.f), 0, EasingType::sineEaseOut);
     SecondModel->animation->AddRotationKeyFrame(glm::quat(0.0f, 0.f, 1.0f, 0.f), 1, EasingType::sineEaseOut);
     SecondModel->animation->AddRotationKeyFrame(glm::quat(0.0f, 1.0f, 0.0f, 0.f), 2, EasingType::sineEaseOut);
     SecondModel->animation->AddRotationKeyFrame(glm::quat(0.0f, 0.0f, 1.0f, 0.f), 3, EasingType::sineEaseOut);

     SecondModel->animation->AddScaleKeyFrame(glm::vec3(1, 1, 1), 0, EasingType::sineEaseOut);
   //  SecondModel->animation->AddScaleKeyFrame(glm::vec3(1, 1.5f, 1), 1, EasingType::sineEaseOut);

  //   SecondModel->animation->AddScaleKeyFrame(glm::vec3(2.5f, 2.5f, 2.5f), 2, EasingType::sineEaseOut);
     SecondModel->animation->AddScaleKeyFrame(glm::vec3(15, 15,15), 5, EasingType::sineEaseOut);



     SecondModel->animation->AddColoreKeyFrame(glm::vec3(1, 1,1), 0, EasingType::sineEaseOut);
     SecondModel->animation->AddColoreKeyFrame(glm::vec3(0, 1,0), 1, EasingType::sineEaseOut);
     SecondModel->animation->AddColoreKeyFrame(glm::vec3(0, 0,1), 1, EasingType::sineEaseOut);


     
     AnimationSequence* anim1 = new AnimationSequence();

     anim1->AddAnimationClip(dir->animation, dir);
   //  anim1->AddAnimationClip(dir->animation, dir);
   //  anim1->AddAnimationClip(SecondModel->animation, SecondModel);
     AnimationSequence* anim2 = new AnimationSequence();

     anim2->AddAnimationClip(SecondModel->animation, SecondModel);

   //  AnimationSystem::GetInstance().AddAnimationSequence(anim1);
  //   AnimationSystem::GetInstance().AddAnimationSequence(anim2);
     AnimationSystem::GetInstance().SetSequence(anim2);


     std::cout << "Animation Time frame :: " << anim1->GetTotalTimeFrame();




#pragma endregion

}

void ApplicationRenderer::PreRender()
{
    glm::mat4 _projection = glm::perspective(glm::radians(camera.Zoom), (float)windowWidth / (float)WindowHeight, 0.1f, 100.0f);
    glm::mat4 _view = camera.GetViewMatrix();
    glm::mat4 _skyboxview = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    glDepthFunc(GL_LEQUAL);
    SkyboxShader->Bind();
    SkyboxShader->setMat4("view", _skyboxview);
    SkyboxShader->setMat4("projection", _projection);

    render.SkyBoxModel->Draw(*SkyboxShader);
    glDepthFunc(GL_LESS);


    defaultShader->Bind();
    // material.SetMaterialProperties(*defaultShader);
    lightManager.UpdateUniformValuesToShader(defaultShader);
    //  lightManager.UpdateUniformValues(defaultShader->ID);


    defaultShader->setMat4("projection", _projection);
    defaultShader->setMat4("view", _view);
    defaultShader->setVec3("viewPos", camera.transform.position.x, camera.transform.position.y, camera.transform.position.z);
    defaultShader->setFloat("time", scrollTime);
    defaultShader->setBool("isDepthBuffer", false);

    alphaBlendShader->Bind();
    lightManager.UpdateUniformValuesToShader(alphaBlendShader);
    alphaBlendShader->setMat4("projection", _projection);
    alphaBlendShader->setMat4("view", _view);
    alphaBlendShader->setVec3("viewPos", camera.transform.position.x, camera.transform.position.y, camera.transform.position.z);
    alphaBlendShader->setFloat("time", scrollTime);
    alphaBlendShader->setBool("isDepthBuffer", false);

    alphaCutoutShader->Bind();
    lightManager.UpdateUniformValuesToShader(alphaCutoutShader);
    alphaCutoutShader->setMat4("projection", _projection);
    alphaCutoutShader->setMat4("view", _view);
    alphaCutoutShader->setVec3("viewPos", camera.transform.position.x, camera.transform.position.y, camera.transform.position.z);
    alphaCutoutShader->setFloat("time", scrollTime);
    alphaCutoutShader->setBool("isDepthBuffer", false);

    SolidColorShader->Bind();
    SolidColorShader->setMat4("projection", _projection);
    SolidColorShader->setMat4("view", _view);

    StencilShader->Bind();
    StencilShader->setMat4("projection", _projection);
    StencilShader->setMat4("view", _view);

    /* ScrollShader->Bind();
       ScrollShader->setMat4("ProjectionMatrix", _projection);*/

}

void ApplicationRenderer::Render()
{
   
    Start();
  
    EditorLayout::GetInstance().InitializeEditors();

    Time::GetInstance().lastFrame = glfwGetTime();
   // glEnable(GL_BLEND);
  //  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
    while (!glfwWindowShouldClose(window))
    {

        Time::GetInstance().SetCurrentTime(glfwGetTime());
       
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        scrollTime += deltaTime;

        ProcessInput(window);

        // Imgui

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        PanelManager::GetInstance().Update((float)windowWidth, (float)WindowHeight);

        ImGui::Render();

        Clear();

        PreRender(); //Update call BEFORE  DRAW
        
         // make models that it should not write in the stencil buffer
         render.Draw();

         EntityManager::GetInstance().Update(Time::GetInstance().deltaTime);

         PostRender(); // Update Call AFTER  DRAW

         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void ApplicationRenderer::PostRender()
{


    m_FrameNumber++;
    if (m_FrameNumber > 1000) m_FrameNumber = 0;
    float val = (float)m_FrameNumber / 250.f;


   
      //  AnimationSystem::GetInstance().Update(Time::GetInstance().deltaTime);
        AnimationSystem::GetInstance().UpdateAnimationSequence(Time::GetInstance().deltaTime);
   
}


void ApplicationRenderer::Clear()
{
    GLCALL(glClearColor(0.1f, 0.1f, 0.1f, 0.1f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void ApplicationRenderer::ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed=25;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime * cameraSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime * cameraSpeed);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime * cameraSpeed);

    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime * cameraSpeed);

    }


}

 void ApplicationRenderer::SetViewPort(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

 void ApplicationRenderer::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
 {  
         if (key == GLFW_KEY_V && action == GLFW_PRESS)
         {

            
             std::cout << "V pressed" << std::endl;

             std::vector<Model*> listOfModels = render.GetModelList();
            
          

             selectedModelCount++;

             if (selectedModelCount > listOfModels.size()-1)
             {
                 selectedModelCount = 0;
             }

            
             render.selectedModel = listOfModels[selectedModelCount];


         }
     
         if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
         {
             AnimationSystem::GetInstance().PlayOrPause();
         }
         if (key == GLFW_KEY_1 && action == GLFW_PRESS)
         {
             AnimationSystem::GetInstance().NextSequence();
         }
 }

 void ApplicationRenderer::MouseCallBack(GLFWwindow* window, double xposIn, double yposIn)
 {

    float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);
     
        if (firstMouse)
        {

        }

         if (firstMouse)
         {
             lastX = xpos;
             lastY = ypos;
             firstMouse = false;
         }
     
         float xoffset = xpos - lastX;
         float yoffset = lastY - ypos;
     
         lastX = xpos;
         lastY = ypos;
     
         if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
         {
             camera.ProcessMouseMovement(xoffset, yoffset);
         }
 }

 void ApplicationRenderer::MouseScroll(GLFWwindow* window, double xoffset, double yoffset)
 {
     camera.ProcessMouseScroll(static_cast<float>(yoffset));
 }
