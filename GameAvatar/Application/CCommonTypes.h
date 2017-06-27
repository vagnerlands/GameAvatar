#ifndef _COMMONTYPES_H_
#define _COMMONTYPES_H_

#include <memory>
#include <list>
#include <vector>
#include <unordered_map>
#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GL/glut.h"
#include "glsl.h"

class IView;
class IViewElement;
class IViewLight;
class CResHandle;
using namespace std;

namespace Types {
	typedef unsigned int TUInt32;
	typedef int TInt32;
	typedef short TInt16;
	typedef unsigned short TUInt16;
	typedef char TByte;
	typedef unsigned char TUByte;
	typedef double TDouble;
	typedef float TFloat;

	static const TByte* s_GAME_CONNECTION_PORT = "1234";

	enum GameViewType
	{
		GameView_Undefined,
		GameView_Human,
		GameView_Remote,
		GameView_AI,
		GameView_Recorder,
		GameView_Other
	};

	enum GameViewElement
	{
		GameViewElement_Undefined,
		GameViewElement_Square,
		GameViewElement_Model,
		GameViewElement_TerrainMesh,
		GameViewElement_Light_Ambient,
		GameViewElement_Light_Specular
	};

	enum ProcessType
	{
		ProcessType_LoadTexture,
		ProcessType_LoadModel,
		ProcessType_LoadShader
	};


	typedef unsigned int GameViewId;


	class CPoint
	{
	public:
		TInt32 x;
		TInt32 y;
	};

	struct SModelData
	{
		vector<glm::vec3> m_vertices;
		vector<glm::vec2> m_textures;
		vector<glm::vec3> m_normals;
		vector<glm::vec3> m_indices;
	};

	typedef unordered_map<string, cwc::glShader*> ShadersMap;
	typedef unordered_map<string, shared_ptr<IViewElement>> ViewElementMap;
	typedef unordered_map<string, shared_ptr<IViewLight>> ViewLightMap;
	typedef list<shared_ptr<IView>> ViewList;
	typedef unordered_map<string, SModelData> ModelMap;
	typedef unordered_map<string, GLuint> TextureMap;
	typedef unordered_map<string, shared_ptr<CResHandle>> TextureContentMap;
	typedef void(*OnRemoveEvent)(string);

	enum ESocketConnectionStatus
	{
		ESOCKETCONNECTION_OK = 0,
		ESOCKETCONNECTION_FAIL,
		ESOCKETCONNECTION_TOTAL,
	};

	// what is the current state of a given socket
	enum ESocketState
	{
		ESOCKETSTATE_FIRST = 0,
		ESOCKETSTATE_LOGIN = ESOCKETSTATE_FIRST,
		ESOCKETSTATE_LOGIN_MENU, // 1 
		ESOCKETSTATE_CREATE_NEW_CHAR_CLASS, // 2
		ESOCKETSTATE_CREATE_NEW_CHAR_GENDER, // 3
		ESOCKETSTATE_CREATE_NEW_CHAR_RACE, // 3
		// more to be added here
		ESOCKETSTATE_IN_GAME, // 3
		ESOCKETSTATE_LAST
	};

	enum EEventType
	{
		EEVENTTYPE_FIRST,
		EEVENTTYPE_MENU_STATE = EEVENTTYPE_FIRST,
		EEVENTTYPE_ACTOR_ENTERGAME,
		EEVENTTYPE_ACTOR_MOVES,
		EEVENTTYPE_ACTOR_ATTACKS,
		EEVENTTYPE_ACTOR_SAYS,
		EEVENTTYPE_ACTOR_SHOUTS,
		EEVENTTYPE_ILLEGAL,
		EEVENTTYPE_LAST
	};

	enum ELoginMenuOption 
	{
		ELOGINMENUOPTION_CREATE_NEW_CHARACTER = 0,
		ELOGINMENUOPTION_START_GAME,
		ELOGINMENUOPTION_LEAVE_GAME,
		ELOGINMENUOPTION_INVALID
	};

	enum ELoginClassMenuOption
	{
		ELOGINCLASSMENUOPTION_SELECT_WARRIOR = 0,
		ELOGINCLASSMENUOPTION_SELECT_MAGE,
		ELOGINCLASSMENUOPTION_SELECT_PALADIN,
		ELOGINCLASSMENUOPTION_SELECT_NINJA,
		ELOGINCLASSMENUOPTION_SELECT_RETURN_TO_MAIN_MENU,
		ELOGINCLASSMENUOPTION_SELECT_INVALID
	};

	enum ELoginGenderMenuOption
	{
		ELOGINGENDERMENUOPTION_SELECT_MALE = 0,
		ELOGINGENDERMENUOPTION_SELECT_FEMALE,
		ELOGINGENDERMENUOPTION_SELECT_RETURN_TO_MAIN_MENU,
		ELOGINGENDERMENUOPTION_SELECT_GENDER_INVALID
	};

	enum ELoginRaceMenuOption
	{
		ELOGINRACEMENUOPTION_SELECT_HUMAN = 0,
		ELOGINRACEMENUOPTION_SELECT_ELF,
		ELOGINRACEMENUOPTION_SELECT_ORC,
		ELOGINRACEMENUOPTION_SELECT_DWARF,
		ELOGINRACEMENUOPTION_SELECT_RETURN_TO_MAIN_MENU,
		ELOGINRACEMENUOPTION_SELECT_INVALID
	};

	enum ESocketCommandType
	{
		ESOCKETCOMMANDTYPE_LOGIN_INJECTED = 0,
		ESOCKETCOMMANDTYPE_TOTAL
	};

	enum ECharacterGender
	{
		ECHARACTERGENDER_MALE = 0,
		ECHARACTERGENDER_FEMALE,
		ECHARACTERGENDER_TOTAL
	};

	enum ECharacterRace
	{
		ECHARACTERRACE_HUMAN = 0,
		ECHARACTERRACE_ELF,
		ECHARACTERRACE_ORC,
		ECHARACTERRACE_DWARF,
		ECHARACTERRACE_TOTAL
	};

	enum ECharacterClass
	{
		ECHARACTERCLASS_WARRIOR = 0,
		ECHARACTERCLASS_MAGE,
		ECHARACTERCLASS_PALADIN,
		ECHARACTERCLASS_NINJA,
		ECHARACTERCLASS_INVALID
	};

	enum EAnsiColor
	{
		EANSICOLOR_BLACK = 0,
		EANSICOLOR_RED,
		EANSICOLOR_GREEN,
		EANSICOLOR_YELLOW,
		EANSICOLOR_BLUE,
		EANSICOLOR_MAGENTA,
		EANSICOLOR_CYAN,
		EANSICOLOR_WHITE
	};
}

#endif //_COMMONTYPES_H_
