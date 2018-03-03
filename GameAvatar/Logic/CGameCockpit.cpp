#include "CGameCockpit.h"
#include "CEventManager.h"
#include "CLandscape.h"
#include "CUserInputEventManager.h"
#ifdef _WIN_
#include "CWinMutex.h"
#endif
#include <time.h>

CGameCockpit* CGameCockpit::s_pInstance = NULL;

CGameCockpit* 
CGameCockpit::instance()
{
	if (s_pInstance == NULL)
	{
		s_pInstance = new CGameCockpit();
	}
	return s_pInstance;
}


void 
CGameCockpit::run()
{
	// limit fps to 60
	clock_t start = clock();

	// prepares MODEL VIEW
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	// Set background (clear) color to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	// notify all controls with last statuses about user input
	m_pGameInputCtrl->OnUpdate();

	// updates the terrain loading area according to camera location 
	// if the location crosses the hysteresis tolerance, it will trigger
	// a new loadings (which will happen in the Loader thread)
	m_pLandscape->SetCameraLocation(
			m_camera.GetCameraAttribute(Types::CameraAttribute_Position)
		);

	// prepares 3D PROJECTION
	m_camera.prepareProjection3D();

	m_camera.SetLookAtMatrix();

	// Notify all controllers with related events - according to CEventManager rules
	CEventManager::instance()->notify();

	//glPushMatrix();
	//m_objSkyBox->Render(0, 0, 0);
	//glPopMatrix();

	// Render 3D Screen objects
	for (ViewList::iterator it = m_views.begin();
			it != m_views.end();
			++it)
	{
		(*it)->VOnRender();
	}

	// 2D elements - no depth test
	m_camera.prepareProjection2D();
	
	// Render 2D Screen objects
	for (HUDList::iterator it = m_huds.begin();
		it != m_huds.end();
		++it)
	{
		(*it)->VPreRender();
		(*it)->VRender();
		(*it)->VPostRender();
	}
	
	// in order the frame rate seems continuous, wait here and hope 
	// OS will give the CPU time for background threads
	while (clock() - start < Types::s_CYCLE_MAX_TIME)
	{
		// no operation
	}

}

void CGameCockpit::ControllerMenuEventHandler(IEvent* ev)
{
	s_pInstance->m_CtrlMenu.execute(ev);
}

void CGameCockpit::UserInputEventHandler(IEvent* ev)
{
	s_pInstance->m_CtrlMenu.execute(ev);
}


void CGameCockpit::pushProcess(shared_ptr<IProcess> pProcess)
{
	m_processesMutex->mutexLock();
	for (list<shared_ptr<IProcess>>::iterator it = m_processes.begin();
		it != m_processes.end();
		++it)
	{
		shared_ptr<IProcess> r = *it;
		if (r->VGetType() == pProcess->VGetType())
		{
			m_processesMutex->mutexUnlock();
			// same process already exists in the queue - then do not add a new one
			return;
		}
	}

	// adds a new process in the queue
	m_processes.push_front(pProcess);
	m_processesMutex->mutexUnlock();
}

shared_ptr<IProcess> CGameCockpit::getNextProcess()
{
	shared_ptr<IProcess> retVal = NULL;
	m_processesMutex->mutexLock();
	if (!m_processes.empty())
	{
		retVal = m_processes.back();
	}
	m_processesMutex->mutexUnlock();
	return retVal;
}

void 
CGameCockpit::popProcess(shared_ptr<IProcess> pProcess)
{
	m_processesMutex->mutexLock();
	for (list<shared_ptr<IProcess>>::iterator it = m_processes.begin();
		m_processes.end() != it;
		++it)
	{
		shared_ptr<IProcess> r = *it;
		if (pProcess == r)
		{
			m_processes.erase(it);
			break;
		}
	}
	m_processesMutex->mutexUnlock();
}

CGameCockpit::CGameCockpit()
{
#ifdef _WIN_
	m_processesMutex = new CWinMutex();
#else
	#error "no implementation for this platform"
#endif
	if (m_processesMutex == NULL)
	{
		cout << "<!> Critical issue while allocating resources, program is being terminated!" << endl;
		exit(1);
	}
	// create a mutex for background processes of this game
	m_processesMutex->createMutex("backgroundProcesses");
	// creates the game control
	shared_ptr<CGameController> pGameControl(new CGameController());
	m_pGameInputCtrl = pGameControl;
	// set the game control to the current view
	shared_ptr<IView> view(new CHumanView());
	view->VSetGameCtrl(m_pGameInputCtrl);
	view->VSetCamera(&m_camera);

	// same definition but for 2D
	// set the game control to the current view
	shared_ptr<IView> view2D(new CHumanView());
	view2D->VSetGameCtrl(m_pGameInputCtrl);
	view2D->VSetCamera(&m_camera);

	// creates an object of Human View
	//shared_ptr<IViewElement> element(new CViewElementSquare(0.0f, 0.0f, 512.0f, 512.0f, "melancia.bmp"));
	//shared_ptr<IViewElement> element2(new CViewElementSquare(-256.0f, -256.0f, 100.0f, 100.0f, "melancia.bmp"));
	shared_ptr<IViewElement> skull(new CViewElementModel(100.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, "skull.obj"));
	shared_ptr<IViewElement> princess(new CViewElementModel(95.0f, 0.0f, 0.0f, 0.1f, 0.1f, 0.1f, "Hughes500.obj"));
	shared_ptr<IViewElement> tree1(new CViewElementModel(105.0f, 0.0f, 0.0f, 0.05f, 0.05f, 0.05f, "Tree low.obj", Types::DrawDirective_Triangle_Fan));
	shared_ptr<IViewElement> tree2(new CViewElementModel(108.0f, 0.0f, 1.0f, 0.05f, 0.05f, 0.05f, "Tree low.obj", Types::DrawDirective_Triangle_Fan));
	shared_ptr<IViewElement> tree3(new CViewElementModel(90.5f, 0.0f, -1.0f, 0.05f, 0.05f, 0.05f, "Tree low.obj", Types::DrawDirective_Triangle_Fan));
	shared_ptr<IViewElement> tree4(new CViewElementModel(88.f, 0.0f, -1.0f, 0.05f, 0.05f, 0.05f, "Tree low.obj", Types::DrawDirective_Triangle_Fan));
	//shared_ptr<IViewElement> mount(new CViewElementModel(13.5f, 0.0f, -1.0f, 1.00f, 1.10f, 1.00f, "castle.obj"));
	shared_ptr<IViewElement> terrain(new CViewElementTerrainMesh(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f));
	shared_ptr<IViewElement2D> hud(new CViewElementHUD(CPoint(-295, 295), CPoint(-150, 100), "comm"));
	hud->VSetTransparency(0.2F);
	//shared_ptr<IViewElement> tiger(new CViewElementModel(0.0f, 0.0f, 2.0f, 2.0f, 2.0f, "tiger.obj"));
	//shared_ptr<IViewElement> master(new CViewElementModel(0.0f, 0.0f, 2.0f, 2.0f, 2.0f, "master.obj"));
	//shared_ptr<IViewElement> monkey(new CViewElementModel(0.0f, 0.0f, 2.0f, 2.0f, 2.0f, "monky.obj"));
	//shared_ptr<IViewElement> bread(new CViewElementModel(0.0f, 0.0f, 2.0f, 2.0f, 2.0f, "ford.obj"));
	//shared_ptr<IViewElement> elementCube2(new CViewElementModel(-150.0f, 0.0f, 2.0f, 2.0f, 2.0f, "skull.obj"));
	//shared_ptr<IViewElement> elementCube3(new CViewElementModel(100.0f, 0.0f, 2.0f, 2.0f, 2.0f, "skull.obj"));

	//view->VPushElement("melan", element);
	//view->VPushElement(element2);
	view->VPushElement("skull", skull);
	view->VPushElement("princess", princess);
	view->VPushElement("terrain", terrain);
	view->VPushElement("t1", tree1);
	view->VPushElement("t2", tree2);
	view->VPushElement("t3", tree3);
	view->VPushElement("t4", tree4);
	//view->VPushElement("mount", mount);
	//view->VPushElement("monkey", monkey);
	//view->VPushElement("tiger", tiger);
	//view->VPushElement("master", master);
	//view->VPushElement("bread", bread);
	//view->VPushElement("skull2", elementCube2);
	//view->VPushElement("skull3", elementCube3);
	//view->VPushElement(element3); 
	//view->VPushElement(element4);
	// push to the list of views
	m_views.push_back(view);

	// input commands
	// TBD: create alias for each key binding - such as: 'fire' == 'a', 'run' == 'b' and so on
	CUserInputEventManager::instance()->RegisterListener(view.get(), 'w', EKeyStatus::KeyStatus_Pressed);
	CUserInputEventManager::instance()->RegisterListener(view.get(), 's', EKeyStatus::KeyStatus_Pressed);
	CUserInputEventManager::instance()->RegisterListener(view.get(), 'a', EKeyStatus::KeyStatus_Pressed);
	CUserInputEventManager::instance()->RegisterListener(view.get(), 'd', EKeyStatus::KeyStatus_Pressed);
	CUserInputEventManager::instance()->RegisterListener(view.get(), 'f', EKeyStatus::KeyStatus_Pressed);
	CUserInputEventManager::instance()->RegisterListener(view.get(), 'g', EKeyStatus::KeyStatus_Pressed);
	CUserInputEventManager::instance()->RegisterListener(view.get(), 'o', EKeyStatus::KeyStatus_Pressed);
	CUserInputEventManager::instance()->RegisterListener(view.get(), 'l', EKeyStatus::KeyStatus_Pressed);
	CUserInputEventManager::instance()->RegisterListener(view.get(), 'k', EKeyStatus::KeyStatus_Pressed);
	CUserInputEventManager::instance()->RegisterListener(view.get(), ';', EKeyStatus::KeyStatus_Pressed);
	CUserInputEventManager::instance()->RegisterListener(view.get(), 'i', EKeyStatus::KeyStatus_Pressed);
	CUserInputEventManager::instance()->RegisterListener(view.get(), 'p', EKeyStatus::KeyStatus_Pressed);
	CUserInputEventManager::instance()->RegisterListener(view.get(), '.', EKeyStatus::KeyStatus_Pressed);
	CUserInputEventManager::instance()->RegisterListener(view.get(), ',', EKeyStatus::KeyStatus_Pressed);
	CUserInputEventManager::instance()->RegisterListener(view.get(), 'z', EKeyStatus::KeyStatus_Pressed);
	CUserInputEventManager::instance()->RegisterListener(view.get(), 'x', EKeyStatus::KeyStatus_Pressed);
	CUserInputEventManager::instance()->RegisterListener(view.get(), 'q', EKeyStatus::KeyStatus_Pressed);
	CUserInputEventManager::instance()->RegisterListener(view.get(), 'e', EKeyStatus::KeyStatus_Pressed);



	m_huds.push_back(hud);
	// Camera Attributes initialization
	m_camera.SetCameraAttribute(CameraAttributeType::CameraAttribute_Position, 100.0f, 70.9750f, 182.f);
	m_camera.SetCameraAttribute(CameraAttributeType::CameraAttribute_Up, 0.0f, 1.0f, 0.f);
	m_camera.SetCameraAttribute(CameraAttributeType::CameraAttribute_Right, 1.0f, 0.0f, 0.f);
	m_camera.SetCameraAttribute(CameraAttributeType::CameraAttribute_Forward, -0.000001f, -0.38975f, -0.92089f);

	m_pLandscape = new CLandscape(30, 32);
	m_pLandscape->Initialize();
	terrain->VSetLandscapeObject(m_pLandscape);

	//m_objSkyBox = new CSkybox();
	//m_objSkyBox->init(90, "./Resources/tex_cube/");

}

void
CGameCockpit::Close()
{

	while (!m_views.empty())
	{
		// shared pointers will automatically deallocate when the last reference is erased
		m_views.pop_back();
	}

	while (!m_processes.empty())
	{
		// shared pointers will automatically deallocate when the last reference is erased
		m_processes.pop_back();
	}

	m_processesMutex->mutexUnlock();
	m_processesMutex->destroy();

	delete m_processesMutex;

	delete s_pInstance;

}

void CGameCockpit::Update()
{
	// updates the terrain situation - if there is any need to update
	m_pLandscape->Update(m_camera);
}


CGameCockpit::~CGameCockpit()
{
	while (!m_views.empty())
	{
		// shared pointers will automatically deallocate when the last reference is erased
		m_views.pop_back();
	}

	s_pInstance = nullptr;
}
