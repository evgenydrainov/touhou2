#include "Stage.h"

#include "Input.h"
#include "Game.h"

#include "mymath.h"
#include "drawing.h"
#include "Text.h"
#include <fmt/format.h>
#include "Log.h"

Stage::Stage()
{
	bullets.reserve(500);
	playerBullets.reserve(100);

	projectiles.loadFromFile("Sprites/Projectiles.png");
	background.loadFromFile("Sprites/Background.png");
	hud.loadFromFile("Sprites/Hud.png");
	characters.loadFromFile("Sprites/Characters.png");
	m_playArea.create(playAreaW, playAreaH);

	std::string script_dir = "Scripts/";
	script.load(script_dir + "stage1.lua");
	m_scriptTexture.loadFromFile(script_dir + "Texture.png");
}

void Stage::update(float delta)
{
	sf::Clock c;

	auto& input = Input::getInstance();

	switch (m_state)
	{
	case State::Normal:
		if (input.checkPressed(Input::Pause))
		{
			pause();
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

void Stage::draw(sf::RenderTarget& target, float delta) const
{
	sf::Clock c;

	m_playArea.clear();

	mDrawAll(delta);
	mDrawPlayAreaHud(delta);

	if (m_pauseMenu)
		m_pauseMenu->draw(m_playArea, delta);

	if (m_gameOverMenu)
		m_gameOverMenu->draw(m_playArea, delta);

	m_playArea.display();

	mDrawBg(target, delta);
	mDrawPlayArea(target, delta);
	mDrawHud(target, delta);

	//Log("Draw: ", c.restart().asSeconds() * 60.0f);
}

void Stage::pause()
{
	m_state = State::Paused;
	m_pauseMenu = new PauseMenu;
}

void Stage::resume()
{
	if (m_pauseMenu)
	{
		delete m_pauseMenu;
		m_pauseMenu = nullptr;
	}

	m_state = State::Normal;
}

void Stage::useContinue()
{
	if (m_gameOverMenu)
	{
		m_gameOverMenu = nullptr;
		delete m_gameOverMenu;
	}

	m_state = State::Normal;
	player = Player();
	player.setAppearingState();
	player.invincibility = 120.0f;
}

void Stage::showGameOverMenu()
{
	m_state = State::GameOver;
	m_gameOverMenu = new GameOverMenu;
}

void Stage::mUpdateAll(float delta)
{
	script.update(delta * gameplayDelta);

	player.update(delta * gameplayDelta);
	if (player.dead)
	{
		showGameOverMenu();
		return;
	}

	for (Bullet& b : bullets)
		b.update(delta * gameplayDelta);

	if (boss)
	{
		if (boss->time_manipulator)
			boss->update(delta);
		else
			boss->update(delta * gameplayDelta);

		if (boss->dead)
			boss = nullptr;
	}

	for (Bullet& pb : playerBullets)
	{
		pb.update(delta * gameplayDelta);
		if (boss)
		{
			pb.direction = math::point_direction(pb.x, pb.y, boss->x, boss->y);
		}
	}
}

void Stage::mPhysicsUpdateAll(float delta)
{
	// ideally, an object shouldn't care whether it's physics simulation is highp or lowp

	constexpr size_t steps = 5;
	constexpr float step = 1.0f / steps;

	float physicsDelta = delta * step;
	
	// highp
	for (size_t i = 0; i < steps; i++)
	{
		for (Bullet& b : bullets)
			b.physicsUpdate(physicsDelta * gameplayDelta);

		player.physicsUpdate(physicsDelta * gameplayDelta);
	}

	// lowp
	for (Bullet& pb : playerBullets)
		pb.physicsUpdate(delta * gameplayDelta);

	if (boss)
		if (boss->time_manipulator)
			boss->physicsUpdate(delta);
		else
			boss->physicsUpdate(delta * gameplayDelta);
}

void Stage::mEndUpdateAll(float delta)
{
	player.endUpdate(delta * gameplayDelta);
	
	for (auto b = bullets.begin(); b != bullets.end(); )
	{
		b->endUpdate(delta * gameplayDelta);
		if (b->dead)
			b = bullets.erase(b);
		else
			++b;
	}

	if (boss)
		if (boss->time_manipulator)
			boss->endUpdate(delta);
		else
			boss->endUpdate(delta * gameplayDelta);

	for (auto pb = playerBullets.begin(); pb != playerBullets.end(); )
	{
		pb->endUpdate(delta * gameplayDelta);
		if (pb->dead)
			pb = playerBullets.erase(pb);
		else
			++pb;
	}
}

void Stage::mDrawAll(float delta) const
{
	player.draw(m_playArea, delta * gameplayDelta);

	if (boss)
	{
		sf::Sprite s;
		s.setTexture(m_scriptTexture);
		s.setTextureRect({ boss->u, boss->v, boss->w, boss->h });
		s.setPosition(boss->x, boss->y);
		s.setOrigin(boss->w / 2, boss->h / 2);
		m_playArea.draw(s);
	}
		//if (boss->time_manipulator)
		//	boss->draw(m_playArea, delta);
		//else
		//	boss->draw(m_playArea, delta * gameplayDelta);

	mBatchBullets(delta * gameplayDelta);

	for (const Bullet& pb : playerBullets)
		draw::circle(m_playArea, pb.x, pb.y, pb.radius, sf::Color::Green);

	auto& game = Game::getInstance();
	if (game.show_hitboxes)
		mDebugDraw(delta);
}

void Stage::mBatchBullets(float delta) const
{
	m_bulletsBuf.clear();
	for (const Bullet& b : bullets)
	{
		//sf::Vector2f p1(b.x + b.w * 0.5f * math::dcos(b.direction - 135.0f), b.y - b.h * 0.5f * math::dsin(b.direction - 135.0f));
		//sf::Vector2f p2(b.x + b.w * 0.5f * math::dcos(b.direction - 45.0f), b.y - b.h * 0.5f * math::dsin(b.direction - 45.0f));
		//sf::Vector2f p4(b.x + b.w * 0.5f * math::dcos(b.direction + 135.0f), b.y - b.h * 0.5f * math::dsin(b.direction + 135.0f));
		//sf::Vector2f p3(b.x + b.w * 0.5f * math::dcos(b.direction + 45.0f), b.y - b.h * 0.5f * math::dsin(b.direction + 45.0f));
		//sf::Vector2f p1(b.x - b.w * 0.5f, b.y - b.h * 0.5f);
		//sf::Vector2f p2(b.x + b.w * 0.5f, b.y - b.h * 0.5f);
		//sf::Vector2f p3(b.x + b.w * 0.5f, b.y + b.h * 0.5f);
		//sf::Vector2f p4(b.x - b.w * 0.5f, b.y + b.h * 0.5f);
		sf::Transform t;
		t.translate(b.x, b.y);
		if (b.rotate)
			t.rotate(-b.direction);
		sf::Vector2f p1 = t.transformPoint(-b.w * 0.5f, -b.h * 0.5f);
		sf::Vector2f p2 = t.transformPoint(b.w * 0.5f, -b.h * 0.5f);
		sf::Vector2f p3 = t.transformPoint(b.w * 0.5f, b.h * 0.5f);
		sf::Vector2f p4 = t.transformPoint(-b.w * 0.5f, b.h * 0.5f);
		m_bulletsBuf.append(sf::Vertex(p1, sf::Vector2f(b.u, b.v)));
		m_bulletsBuf.append(sf::Vertex(p2, sf::Vector2f(b.u + b.w, b.v)));
		m_bulletsBuf.append(sf::Vertex(p3, sf::Vector2f(b.u + b.w, b.v + b.h)));
		m_bulletsBuf.append(sf::Vertex(p4, sf::Vector2f(b.u, b.v + b.h)));
	}
	
	m_playArea.draw(m_bulletsBuf, &m_scriptTexture);

	//for (const Bullet& b : bullets)
	//{
	//	sf::Sprite s;
	//	s.setTexture(m_scriptTexture);
	//	s.setPosition(b.x, b.y);
	//	if (b.rotate)
	//		s.setRotation(-b.direction);
	//	s.setOrigin(b.w / 2, b.h / 2);
	//	s.setTextureRect({ b.u, b.v, b.w, b.h });
	//	m_playArea.draw(s);
	//}
}

void Stage::mDebugDraw(float delta) const
{
	draw::circle(m_playArea, player.x, player.y, player.radius, sf::Color::Green);

	if (boss)
		draw::circle(m_playArea, boss->x, boss->y, boss->radius, sf::Color::Blue);

	for (const Bullet& b : bullets)
		draw::circle(m_playArea, b.x, b.y, b.radius, sf::Color::Red);

	for (const Bullet& pb : playerBullets)
		draw::circle(m_playArea, pb.x, pb.y, pb.radius, sf::Color::Green);
}

void Stage::mDrawPlayAreaHud(float delta) const
{
	if (boss)
	{
		Text phasesLeft;
		phasesLeft.setString(fmt::format("{}", boss->getPhasesLeft()));
		m_playArea.draw(phasesLeft);

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
			m_playArea.draw(back);
			sf::RectangleShape front;
			front.setSize({ ww * boss->hp / boss->getMaxHp(), hh });
			front.setPosition(xx, yy);
			front.setFillColor({ 255, 0, 0 });
			m_playArea.draw(front);
		}

		Text timer;
		timer.setPosition(playAreaW, 0.0f);
		timer.setString(fmt::format("{}", std::ceil(boss->timer)));
		timer.align(Text::HAlign::Right, Text::VAlign::Top);
		m_playArea.draw(timer);

		Text name;
		name.setPosition(0.0f, 16.0f);
		name.setStyle(sf::Text::Italic);
		name.setString(boss->name);
		m_playArea.draw(name);

		Text spellcardName;
	}
}

void Stage::mDrawBg(sf::RenderTarget& target, float delta) const
{
	target.draw(sf::Sprite(background));
}

void Stage::mDrawPlayArea(sf::RenderTarget& target, float delta) const
{
	sf::Sprite s(m_playArea.getTexture());
	s.setPosition(playAreaX, playAreaY);
	target.draw(s);
}

void Stage::mDrawHud(sf::RenderTarget& target, float delta) const
{
	sf::Transform t;
	t.translate(playAreaX + playAreaW + m_hudX, playAreaY + m_hudY);

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
		"{}/{}",
		0,
		player.score,
		player.lives,
		player.bombs,
		power,
		player.graze,
		player.point, 50));
	target.draw(hud, t);

	t.translate(0.0f, 10.0f * 16.0f);

	auto& game = Game::getInstance();
	if (game.show_hitboxes)
	{
		Text debug;
		debug.setStyle(sf::Text::Italic);
		debug.setCharacterSize(12);
		debug.setString(fmt::format(
			"{} bullets\n"
			"capacity: {}",
			bullets.size(),
			bullets.capacity()));
		target.draw(debug, t);
	}
}
