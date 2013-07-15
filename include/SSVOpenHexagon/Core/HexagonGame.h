// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef HG_HEXAGONGAME
#define HG_HEXAGONGAME

#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SSVUtilsJson/SSVUtilsJson.h>
#include <SSVUtils/SSVUtils.h>
#include <SSVStart/SSVStart.h>
#include <SSVEntitySystem/SSVEntitySystem.h>
#include <SSVLuaWrapper/SSVLuaWrapper.h>
#include "SSVOpenHexagon/Core/HGStatus.h"
#include "SSVOpenHexagon/Data/LevelData.h"
#include "SSVOpenHexagon/Data/MusicData.h"
#include "SSVOpenHexagon/Data/StyleData.h"
#include "SSVOpenHexagon/Global/Assets.h"
#include "SSVOpenHexagon/Global/Config.h"
#include "SSVOpenHexagon/Global/Factory.h"
#include "SSVOpenHexagon/Global/Typedefs.h"
#include "SSVOpenHexagon/Utils/FPSWatcher.h"

namespace hg
{
	class MenuGame;

	class HexagonGame
	{
		private:
			ssvs::GameState game;
			ssvs::GameWindow& window;
			sses::Manager manager;
			ssvs::Camera backgroundCamera{window, {{0, 0}, {getWidth() * getZoomFactor(), getHeight() * getZoomFactor()}}};
			ssvs::Camera overlayCamera{window, {{getWidth() / 2.f, getHeight() / 2.f}, ssvs::Vec2f(getWidth(), getHeight())}};
			std::vector<ssvs::Camera> depthCameras;
			ssvu::TimelineManager effectTimelineManager;
			Factory factory{*this, manager, {0, 0}};
			Lua::LuaContext	lua;
			LevelData levelData;
			MusicData musicData;
			StyleData styleData;
			ssvu::Timeline timeline, eventTimeline, messageTimeline;
			sf::Text messageText{"", getFont("imagine.ttf"), static_cast<unsigned int>(38.f / getZoomFactor())};
			sf::VertexArray flashPolygon{sf::PrimitiveType::Quads, 4};
			bool firstPlay{true}, restartFirstTime{true}, inputFocused{false}, inputSwap{false}, mustTakeScreenshot{false}, mustChangeSides{false};
			HexagonGameStatus status;
			std::string restartId{""};
			float difficultyMult{1};
			int inputMovement{0};

			FPSWatcher fpsWatcher;
			sf::Text text{"", getFont("imagine.ttf"), static_cast<unsigned int>(25.f / getZoomFactor())};


			// LUA-related methods
			void initLua();
			void runLuaFile(const std::string& mFileName);
			template<typename R, typename... Args> R runLuaFunction(const std::string& variableName, const Args&... args)
			{
				try { return lua.callLuaFunction<R>(variableName, std::make_tuple(args...)); }
				catch(std::runtime_error &error)
				{
					std::cout << variableName << std::endl << "LUA runtime error: " << std::endl << ssvu::toStr(error.what()) << std::endl << std::endl;
				}

				return R();
			}

			void initFlashEffect();

			// Update methods
			void update(float mFrameTime);
			void updateTimeStop(float mFrameTime);
			void updateIncrement();
			void updateEvents(float mFrameTime);
			void updateLevel(float mFrameTime);
			void updatePulse(float mFrameTime);
			void updateBeatPulse(float mFrameTime);
			void updateRotation(float mFrameTime);
			void updateFlash(float mFrameTime);
			void update3D(float mFrameTime);

			// Draw methods
			void draw();

			// Gameplay methods
			void incrementDifficulty();
			void sideChange(int mSideNumber);

			// Draw methods
			void drawText();

			// Data-related methods
			void setLevelData(LevelData mLevelSettings, bool mMusicFirstPlay);
			void playLevelMusic();
			void stopLevelMusic();

			// Message-related methods
			void addMessage(const std::string& mMessage, float mDuration);

			// Level/menu loading/unloading/changing
			void checkAndSaveScore();
			void goToMenu(bool mSendScores = true);
			void changeLevel(const std::string& mId, bool mFirstTime);

			void invalidateScore();

		public:
			MenuGame* mgPtr;

			HexagonGame(ssvs::GameWindow& mGameWindow);

			// Gameplay methods
			void newGame(const std::string& mId, bool mFirstPlay, float mDifficultyMult);
			void death(bool mForce = false);

			// Other methods
			void executeEvents(ssvuj::Value& mRoot, float mTime);

			// Graphics-related methods
			void render(sf::Drawable&);

			// Setters
			inline void setSpeedMultiplier(float mSpeedMultiplier)	{ levelData.speedMult = mSpeedMultiplier; }
			inline void setDelayMultiplier(float mDelayMultiplier)	{ levelData.delayMult = mDelayMultiplier; }
			inline void setRotationSpeed(float mRotationSpeed)		{ levelData.rotationSpeed = mRotationSpeed; }
			void setSides(unsigned int mSides);

			// Getters
			inline ssvs::GameState& getGame()						{ return game; }
			inline float getRadius() const							{ return status.radius; }
			inline const sf::Color& getColor(int mIndex) const		{ return styleData.getColor(mIndex); }
			inline float getSpeedMultiplier() const					{ return levelData.speedMult * (pow(difficultyMult, 0.65f)); }
			inline float getDelayMultiplier() const					{ return levelData.delayMult / (pow(difficultyMult, 0.10f)); }
			inline float getRotationSpeed() const					{ return levelData.rotationSpeed; }
			inline unsigned int getSides() const					{ return levelData.sides; }
			inline float getWallSkewLeft() const					{ return levelData.wallSkewLeft; }
			inline float getWallSkewRight() const					{ return levelData.wallSkewRight; }
			inline float getWallAngleLeft() const					{ return levelData.wallAngleLeft; }
			inline float getWallAngleRight() const					{ return levelData.wallAngleRight; }
			inline float get3DEffectMult() const					{ return levelData._3dEffectMultiplier; }
			inline HexagonGameStatus& getStatus()					{ return status; }
			sf::Color getColorMain() const;

			// Input
			inline bool getInputFocused() const	{ return inputFocused; }
			inline bool getInputSwap() const	{ return inputSwap; }
			inline int getInputMovement() const	{ return inputMovement; }
	};
}

#endif
