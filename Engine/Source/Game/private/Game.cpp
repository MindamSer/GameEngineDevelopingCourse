#include <Camera.h>
#include <DefaultGeometry.h>
#include <Game.h>
#include <GameObject.h>
#include <Input/InputHandler.h>

namespace GameEngine
{
	Game::Game(
		std::function<bool()> PlatformLoopFunc
	) :
		PlatformLoop(PlatformLoopFunc)
	{
		Core::g_MainCamera = new Core::Camera();
		Core::g_MainCamera->SetPosition(Math::Vector3f(0.0f, 6.0f, -6.0f));
		Core::g_MainCamera->SetViewDir(Math::Vector3f(0.0f, -6.0f, 6.0f).Normalized());

		m_renderThread = std::make_unique<Render::RenderThread>();

		// How many objects do we want to create
		for (int i = 0; i < 100; ++i)
		{
			Math::Vector3f SpawnPos = Math::Vector3f((float)(i % 10) * 5.0f, 0.0f, (float)(i / 10) * 5.0f);

			// Randomly selecting object type
			int ObjectType = rand() % 3;
			if (ObjectType == 0)
			{
				m_Objects.push_back(new SinusGameObject(SpawnPos));
			}
			if (ObjectType == 1)
			{
				m_Objects.push_back(new GravityGameObject(SpawnPos));
			}
			if (ObjectType == 2)
			{
				m_Objects.push_back(new ControlGameObject(SpawnPos));
			}

			Render::RenderObject** renderObject = m_Objects.back()->GetRenderObjectRef();
			m_renderThread->EnqueueCommand(Render::ERC::CreateRenderObject, RenderCore::DefaultGeometry::Cube(), renderObject);
		}

		Core::g_InputHandler->RegisterCallback("GoForward", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetViewDir()); });
		Core::g_InputHandler->RegisterCallback("GoBack", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetViewDir()); });
		Core::g_InputHandler->RegisterCallback("GoRight", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetRightDir()); });
		Core::g_InputHandler->RegisterCallback("GoLeft", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetRightDir()); });

		// Arrows control for cubes
		Core::g_InputHandler->RegisterCallback("CubeGoUp", [&]() {
			for (int i = 0; i < m_Objects.size(); ++i)
			{
				if (dynamic_cast<ControlGameObject*>(m_Objects[i]))
				{
					m_Objects[i]->SetSpeed(Math::Vector3f(0.0f, 1.0f, 0.0f));
				}
			}
			});
		Core::g_InputHandler->RegisterCallback("CubeGoDown", [&]() {
			for (int i = 0; i < m_Objects.size(); ++i)
			{
				if (dynamic_cast<ControlGameObject*>(m_Objects[i]))
				{
					m_Objects[i]->SetSpeed(Math::Vector3f(0.0f, -1.0f, 0.0f));
				}
			}
			});
	}

	void Game::Run()
	{
		assert(PlatformLoop != nullptr);

		m_GameTimer.Reset();

		bool quit = false;
		while (!quit)
		{
			m_GameTimer.Tick();
			float dt = m_GameTimer.GetDeltaTime();

			Core::g_MainWindowsApplication->Update();
			Core::g_InputHandler->Update();
			Core::g_MainCamera->Update(dt);

			Update(dt);

			m_renderThread->OnEndFrame();

			// The most common idea for such a loop is that it returns false when quit is required, or true otherwise
			quit = !PlatformLoop();
		}
	}

	void Game::Update(float dt)
	{
		for (int i = 0; i < m_Objects.size(); ++i)
		{
			m_Objects[i]->Update(dt, m_renderThread->GetMainFrame());
		}
	}
}