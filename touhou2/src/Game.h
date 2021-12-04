#pragma once
#include "Singleton.h"

//#include "TitleScreen.h"
#include "Stage.h"

#include <SFML/Graphics.hpp>

class Game : public Singleton<Game>
{
public:
	constexpr static int gameW = 640;
	constexpr static int gameH = 480;

	enum class Fullscreen
	{
		Disabled,
		Enabled,
		Borderless
	};

	void run();

	void goToTitleScren();
	void startStage();
	void restartStage();
	void quitFromStage();

	void loadOptions();
	void saveOptions();

	void setFullscreen(Fullscreen fullscreen);
	inline Fullscreen getFullscreen() const { return m_fullscreen; }

	void setVSync(bool enabled);
	inline bool getVSync() const { return m_vSync; }

	//TitleScreen* title = nullptr;
	Stage* stage = nullptr;

	sf::Font simsun;

	bool show_hitboxes = false;

private:
	enum class State
	{
		Null,
		Title,
		Gameplay,
	};

	void mTick(float delta);
	void mRender() const;

	State m_state{};

	mutable sf::RenderWindow m_window;
	mutable sf::RenderTexture m_gameSurf;

	float m_displayFps = 0.0f;
	float m_displayFpsCount = 0.0f;
	float m_displayFpsCountN = 0.0f;

	Fullscreen m_fullscreen{};
	bool m_vSync{};
};
