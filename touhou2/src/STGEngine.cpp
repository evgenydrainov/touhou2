#include "STGEngine.h"

#include "Input.h"
#include "Game.h"

#include "mymath.h"
#include "drawing.h"
#include "Text.h"
#include <fmt/format.h>
#include "Log.h"

STGEngine::STGEngine()
{
	projectiles.loadFromFile("Sprites/Projectiles.png");
	background.loadFromFile("Sprites/Background.png");
	hud.loadFromFile("Sprites/Hud.png");
	m_playArea.create(playAreaW, playAreaH);

	m_state = State::Normal;
	m_script.load("Scripts/stage1.lua");
}

void STGEngine::update(float delta)
{
	sf::Clock c;

	auto& input = Input::getInstance();

	switch (m_state)
	{
	case State::Normal:
		if (input.checkPressed(Input::Pause))
		{
			m_state = State::Paused;
			m_pauseMenu = std::make_unique<PauseMenu>();
			break;
		}

		mUpdateAll(delta);
		mPhysicsUpdateAll(delta);
		mEndUpdateAll(delta);
		break;

	case State::Paused:
		m_pauseMenu->update(delta);
		break;

	case State::GameOver:
		m_gameOverMenu->update(delta);
		break;
	}

	//Log("Bullets: ", bullets.size());
	//Log("Update: ", c.restart().asSeconds() * 60.0f);
}

void STGEngine::draw(sf::RenderTarget& target, float delta) const
{
	sf::Clock c;
	
	m_playArea.clear();
	mDrawAll(delta);
	
	if (m_pauseMenu)
		m_pauseMenu->draw(m_playArea, delta);

	if (m_gameOverMenu)
		m_gameOverMenu->draw(m_playArea, delta);

	if (m_show_hitboxes)
		mDebugDraw(delta);

	m_playArea.display();

	mDrawBg(target, delta);
	mDrawPlayArea(target, delta);
	mDrawHud(target, delta);

	//Log("Draw: ", c.restart().asSeconds() * 60.0f);
}

void STGEngine::resume()
{
	m_pauseMenu = nullptr;
	m_state = State::Normal;
}

void STGEngine::useContinue()
{
	m_gameOverMenu = nullptr;
	m_state = State::Normal;
	player = Player();
	player.setAppearingState();
}

void STGEngine::mUpdateAll(float delta)
{
	m_script.update(delta);

	player.update(delta);
	if (player.dead)
	{
		m_state = State::GameOver;
		m_gameOverMenu = std::make_unique<GameOverMenu>();
		return;
	}

	for (Bullet& b : bullets)
		b.update(delta);

	if (boss)
	{
		boss->update(delta);
		if (boss->dead)
			boss = nullptr;
	}

	for (Bullet& pb : playerBullets)
		pb.update(delta);
}

void STGEngine::mPhysicsUpdateAll(float delta)
{
	// ideally, an object shouldn't care whether it's physics simulation is highp or lowp

	constexpr size_t steps = 5;
	constexpr float step = 1.0f / steps;

	float physicsDelta = delta * step;
	
	// highp
	for (size_t i = 0; i < steps; i++)
	{
		for (Bullet& b : bullets)
			b.physicsUpdate(physicsDelta);

		player.physicsUpdate(physicsDelta);
	}

	// lowp
	for (Bullet& pb : playerBullets)
		pb.physicsUpdate(delta);

	if (boss)
		boss->physicsUpdate(delta);
}

void STGEngine::mEndUpdateAll(float delta)
{
	player.endUpdate(delta);
	
	for (auto b = bullets.begin(); b != bullets.end(); )
	{
		b->endUpdate(delta);
		if (b->dead)
			b = bullets.erase(b);
		else
			++b;
	}

	if (boss)
		boss->endUpdate(delta);

	for (auto pb = playerBullets.begin(); pb != playerBullets.end(); )
	{
		pb->endUpdate(delta);
		if (pb->dead)
			pb = playerBullets.erase(pb);
		else
			++pb;
	}
}

void STGEngine::mDrawAll(float delta) const
{
	player.draw(m_playArea, delta);

	if (boss)
		boss->draw(m_playArea, delta);

	mBatchBullets(delta);

	for (const Bullet& pb : playerBullets)
		draw::circle(m_playArea, pb.x, pb.y, pb.radius, sf::Color::Green);
}

void STGEngine::mBatchBullets(float delta) const
{
	m_bulletsBuf.clear();
	for (const Bullet& b : bullets)
	{
		sf::Vector2f p(b.x, b.y);
		sf::Vector2f ox(8.0f, 0.0f);
		sf::Vector2f oy(0.0f, 8.0f);

		sf::Vertex topLeft(p - ox - oy, sf::Vector2f(8.0f, 41.0f));
		sf::Vertex topRight(p + ox - oy, sf::Vector2f((8.0f + 16.0f), 41.0f));
		sf::Vertex bottomRight(p + ox + oy, sf::Vector2f((8.0f + 16.0f), (41.0f + 16.0f)));
		sf::Vertex bottomLeft(p - ox + oy, sf::Vector2f(8.0f, (41.0f + 16.0f)));

		m_bulletsBuf.append(topLeft);
		m_bulletsBuf.append(topRight);
		m_bulletsBuf.append(bottomRight);
		m_bulletsBuf.append(bottomLeft);
	}

	sf::RenderStates s;
	s.texture = &projectiles;
	m_playArea.draw(m_bulletsBuf, s);
}

void STGEngine::mDebugDraw(float delta) const
{
	draw::circle(m_playArea, player.x, player.y, player.radius, sf::Color::Green);

	if (boss)
		draw::circle(m_playArea, boss->x, boss->y, boss->radius, sf::Color::Blue);

	for (const Bullet& b : bullets)
		draw::circle(m_playArea, b.x, b.y, b.radius, sf::Color::Red);

	for (const Bullet& pb : playerBullets)
		draw::circle(m_playArea, pb.x, pb.y, pb.radius, sf::Color::Green);
}

void STGEngine::mDrawBg(sf::RenderTarget& target, float delta) const
{
	target.draw(sf::Sprite(background));
}

void STGEngine::mDrawPlayArea(sf::RenderTarget& target, float delta) const
{
	sf::Sprite s(m_playArea.getTexture());
	s.setPosition(playAreaX, playAreaY);
	target.draw(s);
}

void STGEngine::mDrawHud(sf::RenderTarget& target, float delta) const
{
	sf::Transform t;
	t.translate(playAreaX, playAreaY);

	if (boss)
	{
		Text phasesLeft;
		phasesLeft.setString(fmt::format("{}", boss->getPhasesLeft()));
		target.draw(phasesLeft, t);

		if (boss->getMaxHp() != 0.0f)
		{
			float xx = 32.0f;
			float ww = playAreaW - 2.0f * xx;
			float yy = 4.0f;
			float hh = 16.0f - 2.0f * yy;
			sf::RectangleShape back;
			back.setSize({ ww, hh });
			back.setPosition(xx, yy);
			back.setFillColor({ 100, 0, 0 });
			target.draw(back, t);
			sf::RectangleShape front;
			front.setSize({ ww * boss->hp / boss->getMaxHp(), hh });
			front.setPosition(xx, yy);
			front.setFillColor({ 255, 0, 0 });
			target.draw(front, t);
		}

		Text timer;
		timer.setPosition(playAreaW, 0.0f);
		timer.setString(fmt::format("{}", std::ceil(boss->timer)));
		timer.align(Text::HAlign::Right, Text::VAlign::Top);
		target.draw(timer, t);

		Text name;
		name.setPosition(0.0f, 16.0f);
		name.setStyle(sf::Text::Italic);
		name.setString(boss->name);
		target.draw(name, t);

		Text spellcardName;
	}

	t.translate(playAreaW + m_hudX, m_hudY);

	// sidebar
	//sf::Sprite sidebar;
	//sidebar.setTexture(hud);
	//sidebar.setTextureRect({ 0, 0, 64, 144 });
	Text sidebar;
	sidebar.setString(
		"HiScore\n"
		"Score\n"
		"\n"
		"Player\n"
		"Bomb\n"
		"\n"
		"Power\n"
		"Graze\n"
		"Point");
	target.draw(sidebar, t);

	std::string power;
	if (player.power >= player.powerMax)
		power = "MAX";
	else
		power = fmt::format("{}", player.power);

	Text hud;
	hud.setPosition(64.0f, 0.0f);
	hud.setString(fmt::format(
		"{:0>9}\n"
		"{:0>9}\n"
		"\n"
		"{}\n"
		"{}\n"
		"\n"
		"{}\n"
		"{}\n"
		"{}/{}\n",
		0,
		player.score,
		player.lives,
		player.bombs,
		power,
		player.graze,
		player.point, 50));
	target.draw(hud, t);
}
