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
class IViewElement2D;
class IViewLight;
class CResHandle;
using namespace std;

namespace Types {
	typedef unsigned int UInt32;
	typedef int Int32;
	typedef short Int16;
	typedef unsigned short UInt16;
	typedef char Byte;
	typedef unsigned char UByte;
	typedef double Double;
	typedef float Float;

	static const Byte* s_GAME_CONNECTION_PORT = "1234";
	static const Float s_CYCLE_MAX_TIME = 16;

	static Int32 s_SCREEN_HEIGHT = 600;
	static Int32 s_SCREEN_WIDTH = 600;

	static Int32 s_INVALID_ELEVATION = -9999;

	static const Int32 s_SCREEN_CENTER_X = s_SCREEN_WIDTH / 2;
	static const Int32 s_SCREEN_CENTER_Y = s_SCREEN_HEIGHT / 2;

	static const Float s_PI = 3.14159265359;

	// default distance between points
	static const UInt32 s_DEFAULT_DISTANCE_BETWEEN_HEIGHT_POINTS = 10;

	enum CameraAttributeType
	{
		CameraAttribute_Position,
		CameraAttribute_Up,
		CameraAttribute_Right,
		CameraAttribute_Forward
	};

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
		GameViewElement_Undefined2D,
		GameViewElement_Square,
		GameViewElement_Model,
		GameViewElement_HUD,
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

	enum EVertexBuffers
	{
		VertexBuffer_Vertices,
		VertexBuffer_Normals,
		VertexBuffer_Textures,
		VertexBuffer_Colors,
		VertexBuffer_Element,
		VertexBuffer_Max_Num
	};

	enum EDrawDirective
	{
		DrawDirective_Triangles, //GL_TRIANGLES
		DrawDirective_Triangle_Strip, //GL_TRIANGLE_STRIP
		DrawDirective_Triangle_Fan, //GL_TRIANGLE_FAN
		DrawDirective_Max_Num
	};


	typedef unsigned int GameViewId;

	struct SLoDInfo
	{
		UInt32 m_bandWidth;
		UInt32 m_levelOfDetailTable[32];
	};

	struct STileInfo
	{
		// current level of detail
		// always a positive number, following the (2^x)-1
		// ex.: 0, 1, 3, 7, 15, 31, ...
		UInt32 m_lod;
		// how many triangles prepared for this tile
		UInt32 m_trianglesCount;
		// vertices database
		glm::vec3* m_vertices;
		// normals
		glm::vec3* m_normals;
		// texture coordinates
		glm::vec2* m_textureCoords;
	};


	class CCoordinates
	{
	public:
		Float m_latitude;
		Float m_longitude;

		CCoordinates()
		{
			this->m_latitude = 0.0f;
			this->m_longitude = 0.0f;
		}

		CCoordinates(Float latitude, Float longitude)
		{
			this->m_latitude = latitude;
			this->m_longitude = longitude;
		}

		CCoordinates operator+(CCoordinates& other)
		{
			return CCoordinates(this->m_latitude + other.m_latitude, this->m_longitude + other.m_longitude);
		}

		CCoordinates operator-(CCoordinates& other)
		{
			return CCoordinates(this->m_latitude - other.m_latitude, this->m_longitude - other.m_longitude);
		}

		CCoordinates& operator=(CCoordinates& other)
		{
			this->m_latitude = other.m_latitude;
			this->m_longitude = other.m_longitude;
			return *this;
		}

		CCoordinates& operator=(const CCoordinates& other)
		{
			this->m_latitude = other.m_latitude;
			this->m_longitude = other.m_longitude;
			return *this;
		}

		bool operator==(CCoordinates& other)
		{
			bool retVal = false;
			if ((this->m_latitude > other.m_latitude - 0.001) && (this->m_latitude < other.m_latitude + 0.001)
				&& (this->m_longitude > other.m_longitude - 0.001) && (this->m_longitude < other.m_longitude + 0.001))
				retVal = true;

			return retVal;
		}

	};


	class CPoint
	{
	public:
		Int32 x;
		Int32 y;

		CPoint() :
			x(0),
			y(0)
		{

		}

		CPoint(CPoint& newPoint) :
			x(newPoint.x),
			y(newPoint.y)
		{

		}

		CPoint(Int32 argX, Int32 argY) :
			x(argX),
			y(argY)
		{

		}

		CPoint& operator=(const CPoint& arg) 			
		{
			x = arg.x;
			y = arg.y;
			return *this;
		}

		CPoint& operator=(CPoint& arg)
		{
			x = arg.x;
			y = arg.y;
			return *this;
		}

		bool operator!=(const CPoint& arg)
		{
			if ((arg.x == x) && (arg.y == y))
			{
				return false;
			}
			return true;
		}

		bool operator==(const CPoint& arg)
		{
			if ((arg.x == x) && (arg.y == y))
			{
				return true;
			}
			return false;
		}
	};

	//! minimum boundary rectangle
	//! defined by 2 points
	//! this MBR should be used only for 2D plans - recommend to be extended
	class CMbr
	{
	public:
		CCoordinates m_bottomLeft;
		CCoordinates m_topRight;

		CMbr() :
			m_bottomLeft(0.F, 0.F), m_topRight(0.F, 0.F)
		{
			
		}

		CMbr(const CCoordinates bottomLeft, const CCoordinates topRight)
			: m_bottomLeft(bottomLeft), m_topRight(topRight)
		{			
			
		}

		bool Contains(const CMbr& second)
		{
			bool retVal = false;

			// checks if the given MBR is inside 
			if ((this->m_bottomLeft.m_latitude <= second.m_bottomLeft.m_latitude) &&
				(this->m_bottomLeft.m_longitude <= second.m_bottomLeft.m_longitude) &&
				(this->m_topRight.m_latitude >= second.m_topRight.m_latitude) &&
				(this->m_topRight.m_longitude >= second.m_topRight.m_longitude))
			{
				retVal = true;
			}

			return retVal;
		}

		bool Contains(const CCoordinates& point)
		{
			bool retVal = false;

			// checks if the given POINT is inside 
			if ((this->m_bottomLeft.m_latitude <= point.m_latitude) &&
				(this->m_bottomLeft.m_longitude <= point.m_longitude) &&
				(this->m_topRight.m_latitude > point.m_latitude) &&
				(this->m_topRight.m_longitude > point.m_longitude))
			{
				retVal = true;
			}

			return retVal;
		}


		

	};

	struct SMaterialAttr
	{
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		Double m_reflect;
		Double m_refract;
		Double m_trans;
		Double m_shiny;
		Double m_glossy;
	};

	struct SFaceAttr
	{
		// vec4 since it supports triangles or quads
		glm::vec4 m_vertexIndex;
		glm::vec4 m_normalIndex;
		glm::vec4 m_textureIndex;
		Int32 m_vertexCount;
		Int32 m_materialIndex;
	};

	struct SModelData
	{
		~SModelData()
		{
			if (m_vboBufferCreated)
			{
				glDeleteVertexArrays(1, &m_vertexArrayObject);
				m_vertexArrayObject = 0;
				
				glDeleteBuffers(VertexBuffer_Max_Num, m_elementBuffer);

				m_vboBufferCreated = false;
			}
		}

		SModelData() : m_vboBufferCreated(false), m_vertexArrayObject(0)
		{
			for (Int32 i = 0; i < VertexBuffer_Max_Num; i++)
			{
				m_elementBuffer[i] = 0;
			}
		}

		bool m_vboBufferCreated;
		GLuint m_vertexArrayObject;
		vector<GLushort> m_indexes;
		//vector<GLushort> m_normalsIndexed;
		//vector<GLushort> m_texturesIndexed;
		// indexes for Normals, Textures, Vertices and Colors
		//GLuint m_vertexBuffer[VertexBuffer_Max_Num];
		GLuint m_elementBuffer[VertexBuffer_Max_Num];

		vector<glm::vec2> m_textures;
		vector<glm::vec3> m_normals;
		vector<glm::vec3> m_vertices;
		vector<SMaterialAttr> m_material;
		//vector<SFaceAttr> m_faces;
	};

	typedef unordered_map<string, cwc::glShader*> ShadersMap;
	typedef unordered_map<string, shared_ptr<IViewElement>> ViewElementMap;
	typedef unordered_map<string, shared_ptr<IViewLight>> ViewLightMap;
	typedef std::list<shared_ptr<IView>> ViewList;
	typedef std::list<shared_ptr<IViewElement2D>> HUDList;
	typedef unordered_map<string, SModelData> ModelMap;
	typedef unordered_map<string, GLuint> TextureMap;
	typedef unordered_map<string, Types::Byte*> TextureContentMap;
	typedef void(*OnRemoveEvent)(string);

	enum EKeyStatus
	{
		KeyStatus_Pressed = 0,
		KeyStatus_Released = 1
	};


	struct SFontAttributes
	{
		SFontAttributes(Int32 width, Int32 height, void* id) :
			m_fontWidth(width),
			m_fontHeight(height),
			m_fontId(id)
		{

		}
		Int32 m_fontWidth;
		Int32 m_fontHeight;
		void* m_fontId;
	};

	typedef unordered_map<string, SFontAttributes> FontMap;

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
