/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: main.cpp
Purpose: main function.
Language: c++
Platform: Window x64
Project: sangmin.kim_CS300_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2021/10/01
End Header --------------------------------------------------------*/

#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "Client.h"
#include "InputManager.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

#include "As1.h"

int main(int /*argc*/, char* /*argv*/[]) {

	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	const char* title = "Sangmin.Kim Graphic Project";
	int width = Client::windowWidth,
		height = Client::windowHeight;
	SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, width, height,
		SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	bool traverse_mode = false;
	bool move_faster_mode = false;

	// GLEW: get function bindings (if possible)
	GLenum value = glewInit();
	if (value != GLEW_OK) {
		std::cout << glewGetErrorString(value) << std::endl;
		SDL_GL_DeleteContext(context);
		SDL_Quit();
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init();
	//std::memset(inputTextBuf, 0, 100);
	//bool check = false;
	//bool check2 = false;
	

	// animation loop
	try {
		Client* client = new Client();
		client->Set_Window_WH(width, height);
		Uint32 ticks_last = SDL_GetTicks();
		bool done = false;
		const char* currentObjFile = "bunny";

		//float* planePos = reinterpret_cast<float*>(&client->as1->plane->Get_Obj_Pos());

		while (!done) {

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame(window);
			ImGui::NewFrame();

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.


			if(ImGui::TreeNode("Obj related"))
			{
				if (ImGui::Checkbox("CalcUVonGPU", &Graphic::instance->isCalcUVonGPU))
				{

				}

				if (ImGui::TreeNode("UV load method"))
				{
					if (ImGui::Button("position"))
					{
						//Reload shader
						Graphic::instance->isLoadUVwithPos = true;
					}
					if (ImGui::Button("normal"))
					{
						//Reload shader
						Graphic::instance->isLoadUVwithPos = false;
					}



					//ImGui::Checkbox("position", &Graphic::instance->isLoadUVwithPos);
					ImGui::TreePop();
				}
				ImGui::Checkbox("NormalDisplay", &client->as1->isPlayingNormal);
				ImGui::Checkbox("FaceNormalDisplay", &client->as1->isPlayingFaceNormal);


				if (ImGui::TreeNode("UV Tree"))
				{
					const char* items[] = { "cylindrical", "spherical", "planar", "cube" };
					static int item_current_idx = 0; // Here we store our selection data as an index.
					if (ImGui::BeginListBox("list"))
					{
						for (int n = 0; n < IM_ARRAYSIZE(items); n++)
						{
							const bool is_selected = (item_current_idx == n);
							if (ImGui::Selectable(items[n], is_selected))
								item_current_idx = n;

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndListBox();
					}

					if (ImGui::Button("ReloadObj"))
					{
						//Reload shader

						if (strcmp(items[item_current_idx], "cylindrical") == 0)
						{
							client->as1->ReloadObj(currentObjFile, Mesh::UVType::CYLINDRICAL_UV);
							Graphic::instance->texType = Mesh::UVType::CYLINDRICAL_UV;
						}
						else if (strcmp(items[item_current_idx], "spherical") == 0)
						{
							client->as1->ReloadObj(currentObjFile, Mesh::UVType::SPHERICAL_UV);
							Graphic::instance->texType = Mesh::UVType::SPHERICAL_UV;
						}
						else if (strcmp(items[item_current_idx], "planar") == 0)
						{
							client->as1->ReloadObj(currentObjFile, Mesh::UVType::PLANAR_UV);
							Graphic::instance->texType = Mesh::UVType::PLANAR_UV;
						}
						else if (strcmp(items[item_current_idx], "cube") == 0)
						{
							client->as1->ReloadObj(currentObjFile, Mesh::UVType::CUBE_MAPPED_UV);
							Graphic::instance->texType = Mesh::UVType::CUBE_MAPPED_UV;
						}
					}


					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
			
			if(ImGui::TreeNode("Scenes"))
			{
				if (ImGui::Button("FirstScene"))
				{
					//Reload shader
					client->as1->SetToFirstScene();
				}
				if (ImGui::Button("SecondScene"))
				{
					//Reload shader
					client->as1->SetToSecondScene();
				}
				if (ImGui::Button("ThirdScene"))
				{
					//Reload shader
					client->as1->SetToThirdScene();
				}

				if (ImGui::Button("LightSwitch"))
				{
					//Reload shader
					client->as1->ToggleLightsInfo();
				}
				ImGui::TreePop();
			}
			
			Object* obj = Graphic::objects[0];

			if(ImGui::TreeNode("Global"))
			{
				ImGui::SliderFloat("shininess", &obj->obj_mat->shiness, 0.1f, 250.f);
				ImGui::ColorEdit3("emissive", &obj->emissive.x, ImGuiColorEditFlags_DefaultOptions_);
				ImGui::ColorEdit3("fogColor", &Graphic::instance->airColor.x, ImGuiColorEditFlags_DefaultOptions_);
				ImGui::ColorEdit3("ambient", &Graphic::instance->ambientColor.x, ImGuiColorEditFlags_DefaultOptions_);

				ImGui::SliderFloat3("globalAtt",
					reinterpret_cast<float*>(&Graphic::instance->globalAtts),
					-3.f, 3.f);

				ImGui::SliderFloat("fogNear", &Graphic::instance->zNear, 0.f, 5.f);
				ImGui::SliderFloat("fogFar", &Graphic::instance->zFar, 0.5f, 20.f);


				ImGui::TreePop();
			}

			
			if (ImGui::TreeNode("ObjList Tree"))
			{
				const char* items[] = { "rhino", "bunny", "bunny_high_poly", "4Sphere", "cup",
				"lucy_princeton", "starwars1"};
				static int item_current_idx = 0; // Here we store our selection data as an index.
				if (ImGui::BeginListBox("ObjList"))
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						const bool is_selected = (item_current_idx == n);
						if (ImGui::Selectable(items[n], is_selected))
							item_current_idx = n;

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndListBox();
				}

				if (ImGui::Button("ReloadObj"))
				{
					//Reload shader
					client->as1->ReloadObj(items[item_current_idx]);
					currentObjFile = items[item_current_idx];
				}


				ImGui::TreePop();
			}

			


			if (ImGui::TreeNode("ShaderList Tree"))
			{
				const char* items[] = { "PhongLighting", "PhongShading", "BlinnShading" };
				static int item_current_idx = 0; // Here we store our selection data as an index.
				if (ImGui::BeginListBox("ShaderList"))
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						const bool is_selected = (item_current_idx == n);
						if (ImGui::Selectable(items[n], is_selected))
							item_current_idx = n;

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndListBox();
				}

				if (ImGui::Button("ReloadObj"))
				{
					//Reload shader
					client->as1->ReloadShader(items[item_current_idx]);
				}


				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Lights"))
			{
				const std::vector<Object*>& objs = Graphic::instance->objects;
				const size_t objSize = objs.size();
				std::vector<Light> lightVec;

				for (size_t i = 0; i < objSize; ++i)
				{
					if (objs[i]->isLightObj)
					{
						std::string index = std::to_string(i);
						if (ImGui::TreeNode(index.c_str()))
						{
							Light& objLight = objs[i]->lightProperty;
							float* ambientVal = (float*)&objLight.ambient;
							float* diffuseVal = (float*)&objLight.diffuse;
							float* specVal = (float*)&objLight.specular;
							
							ImGui::ColorEdit3("ambient", ambientVal, ImGuiColorEditFlags_DefaultOptions_);
							ImGui::ColorEdit3("diffuse", diffuseVal, ImGuiColorEditFlags_DefaultOptions_);
							ImGui::ColorEdit3("specular", specVal, ImGuiColorEditFlags_DefaultOptions_);

							ImGui::SliderInt("speed", &objs[i]->orbitRotatingSpeed, 0, 40);

							if(ImGui::Button("Stop"))
							{
								objs[i]->orbitRotatingSpeed = 0;
							}

							if (ImGui::BeginListBox("LightList"))
							{
								if(ImGui::Button("Directional"))
								{
									objs[i]->lightKind = LightKinds::DirectionalLight;
								}
								if (ImGui::Button("Point"))
								{
									objs[i]->lightKind = LightKinds::PointLight;
								}
								if (ImGui::Button("Spot"))
								{
									objs[i]->lightKind = LightKinds::SpotLight;
								}
								
								ImGui::EndListBox();
							}

							ImGui::SliderInt("Position", &objs[i]->orbitIndex, 0, 9999);

							switch(objs[i]->lightKind)
							{
							case LightKinds::DirectionalLight:
								break;
							case LightKinds::PointLight:
								break;
							case LightKinds::SpotLight:
								ImGui::SliderFloat2("CutOffs", &objs[i]->cutoffs.x, 0.f, 10.f);
								break;

							default: ;
							}


							ImGui::TreePop();
						}

						
					}
				}

				//here

				


				ImGui::TreePop();
			}
			
			ImGui::SliderFloat("Fresnel", &Graphic::instance->fersnelVal, 0.f, 100.f);

			if (ImGui::Button("AddLight"))
			{
				//Add light
				client->as1->AddLight();
			}
			if (ImGui::Button("DeleteLight"))
			{
				//Delete light
				client->as1->DeleteLight();
			}

			if (ImGui::Checkbox("ReflectOnly", &Graphic::instance->isReflectOnly))
			{

			}
			if (ImGui::Checkbox("RefractOnly", &Graphic::instance->isRefractOnly))
			{

			}

			ImGui::End();

			SDL_Event event;
			while (SDL_PollEvent(&event)) 
			{
				ImGui_ImplSDL2_ProcessEvent(&event);
				switch (event.type) {
				case SDL_QUIT:
					done = true;
					break;
				case SDL_KEYDOWN:
				{
					const SDL_Keycode code = event.key.keysym.sym;

					InputManager::instance->Press(code);
						
					if (code == SDLK_ESCAPE)
						done = true;
					else if (code == SDLK_SPACE)
					{
						traverse_mode = !traverse_mode;
						client->Set_Traverse_Mode(traverse_mode);
					}
					else if (code == SDLK_LSHIFT)
					{
						move_faster_mode = !move_faster_mode;
						client->Set_Move_Faster(move_faster_mode);
					}
					else if (code == SDLK_w ||
						code == SDLK_a ||
						code == SDLK_s ||
						code == SDLK_d)
					{
						client->Move(code);
					}
					else if (code == SDLK_RIGHT)
					{
						//client->Increase_Graphic_Level();
					}
					else if (code == SDLK_LEFT)
					{
						//client->Decrease_Graphic_Level();
					}
				}
					break;
				case SDL_MOUSEWHEEL:
					client->mousewheel(event);
					break;
				case SDL_MOUSEBUTTONDOWN:
					client->mousepress();
					break;
				case SDL_MOUSEMOTION:
					int mouse_x, mouse_y;

					SDL_GetMouseState(&mouse_x, &mouse_y);

					if (traverse_mode == true)
					{
						client->Update_AB(Point(static_cast<float>(mouse_x), static_cast<float>(mouse_y), 0));
					}
					client->mouse_motion();
					client->Set_Prev_Mousepos(Point(static_cast<float>(mouse_x), static_cast<float>(mouse_y), 0));
					break;
				case SDL_MOUSEBUTTONUP:
					client->Set_Selected_Null();
					break;
				}
			}
			Uint32 ticks = SDL_GetTicks();
			float dt = 0.001f * (ticks - ticks_last);
			ticks_last = ticks;
			client->draw(dt);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			SDL_GL_SwapWindow(window);
		}

		delete client;
	}

	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	SDL_GL_DeleteContext(context);
	SDL_Quit();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return 0;
}

