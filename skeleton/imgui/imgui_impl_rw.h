IMGUI_API bool ImGui_ImplRW_Init(void);
IMGUI_API void ImGui_ImplRW_Shutdown(void);
#ifdef ENABLE_SKELETON
IMGUI_API void ImGui_ImplRW_NewFrame(float timeDelta);
sk::EventStatus ImGuiEventHandler(sk::Event e, void *param);
#else
IMGUI_API void ImGui_ImplRW_NewFrame(float timeDelta,uint32_t w,uint32_t h);
IMGUI_API void ImGui_ImplRW_EndFrame();
#endif
void ImGui_ImplRW_RenderDrawLists(ImDrawData* draw_data);