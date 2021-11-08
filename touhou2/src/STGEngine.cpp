#include "STGEngine.h"

#include "mymath.h"
#include "drawing.h"
#include "Text.h"
#include <fmt/format.h>

#include "Log.h"

#include "Input.h"

// TODO: get rid of this
int cursor = 0;
const char* menu[] =
{
	"Restart",
	"Exit"
};

void STGEngine::loadAssets()
{
	projectiles.loadFromFile("Sprites/Projectiles.png");
	background.loadFromFile("Sprites/Background.png");
	hud.loadFromFile("Sprites/Hud.png");

	m_playArea.create(playAreaW, playAreaH);
}

void STGEngine::initialize()
{
	m_state = State::Normal;
	m_script.load("Scripts/stage1.lua");
}

void STGEngine::update(float delta)
{
	sf::Clock c;

	switch (m_state)
	{
	case State::Normal:
		m_updateAll(delta);
		m_moveAll(delta);
		m_checkBoundsAll();
		break;

	case State::GameOver:
		auto& input = Input::getInstance();
		cursor += input.checkPressed(Input::Down) - input.checkPressed(Input::Up);
		cursor = math::modulo(cursor, 2);
		if (input.checkPressed(Input::Confirm))
			if (cursor == 0)
			{
				// TODO: deal with this somehow
				player = Player();
				boss = nullptr;
				bullets.clear();
				playerBullets.clear();
				initialize();
			}
			else if (cursor == 1)
			{
			}
		break;
	}

	//Log("Bullets: ", bullets.size());
	//Log("Update: ", c.restart().asSeconds() * 60.0f);
}

void STGEngine::draw(sf::RenderTexture& target, float delta) const
{
	sf::Clock c;
	
	m_playArea.clear();
	m_drawAll(delta);
	//debugDraw(delta);

	if (m_state == State::GameOver)
	{
		sf::RectangleShape r;
		r.setSize({ playAreaW, playAreaH });
		r.setFillColor({ 0, 0, 0, 128 });
		m_playArea.draw(r);
		for (int i = 0; i < 2; i++)
		{
			Text t;
			t.setPosition(playAreaW / 2, playAreaH / 3 + 16 * i);
			if (i == cursor)
				t.setFillColor(sf::Color::Yellow);

			t.setString(menu[i]);
			t.align(Text::HAlign::Center, Text::VAlign::Top);
			m_playArea.draw(t);
		}
	}

	m_playArea.display();

	m_drawBg(target, delta);
	m_drawPlayArea(target, delta);
	m_drawHud(target, delta);

	//Log("Draw: ", c.restart().asSeconds() * 60.0f);
}

void STGEngine::m_updateAll(float delta)
{
	m_script.update(delta);

	player.update(delta);
	if (player.isDead())
	{
		m_state = State::GameOver;
		return;
	}

	if (boss)
	{
		boss->update(delta);
		if (boss->isDead())
			boss = nullptr;
	}
}

void STGEngine::m_moveAll(float delta)
{
	constexpr size_t steps = 5;
	constexpr float step = 1.0f / steps;

	float physicsDelta = delta * step;
	
	for (size_t i = 0; i < steps; i++)
	{
		m_moveHighP(physicsDelta);
		m_checkCollisionsHighP();
	}

	m_moveLowP(delta);
	m_checkCollisionsLowP();
}

void STGEngine::m_checkBoundsAll()
{
	player.checkBounds();
	
	for (auto b = bullets.begin(); b != bullets.end(); )
	{
		b->checkBounds();
		if (b->isDead())
			b = bullets.erase(b);
		else
			++b;
	}

	for (auto pb = playerBullets.begin(); pb != playerBullets.end(); )
	{
		pb->checkBounds();
		if (pb->isDead())
			pb = playerBullets.erase(pb);
		else
			++pb;
	}
}

void STGEngine::m_moveHighP(float physicsDelta)
{
	for (Bullet& b : bullets)
		b.move(physicsDelta);

	player.move(physicsDelta);
}

void STGEngine::m_moveLowP(float delta)
{
	for (Bullet& pb : playerBullets)
		pb.move(delta);

	if (boss)
		boss->move(delta);
}

void STGEngine::m_checkCollisionsHighP()
{
	player.checkCollisions();
}

void STGEngine::m_checkCollisionsLowP()
{
	if (boss)
		boss->checkCollisions();
}

void STGEngine::m_drawAll(float delta) const
{
	player.draw(m_playArea, delta);

	if (boss)
		boss->draw(m_playArea, delta);

	m_batchBullets(delta);

	for (const Bullet& pb : playerBullets)
		draw::circle(m_playArea, pb.getX(), pb.getY(), pb.getRadius(), sf::Color::Green);
}

void STGEngine::m_batchBullets(float delta) const
{
	m_bulletsBuf.clear();
	for (const Bullet& b : bullets)
	{
		sf::Vector2f p(b.getX(), b.getY());
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

void STGEngine::m_debugDraw(float delta) const
{
	draw::circle(m_playArea, player.getX(), player.getY(), player.getRadius(), sf::Color::Green);

	for (const Bullet& b : bullets)
		draw::circle(m_playArea, b.getX(), b.getY(), b.getRadius(), sf::Color::Red);
}

void STGEngine::m_drawBg(sf::RenderTarget& target, float delta) const
{
	target.draw(sf::Sprite(background));
}

void STGEngine::m_drawPlayArea(sf::RenderTarget& target, float delta) const
{
	sf::Sprite s(m_playArea.getTexture());
	s.setPosition(playAreaX, playAreaY);
	target.draw(s);
}

void STGEngine::m_drawHud(sf::RenderTexture& target, float delta) const
{
	sf::Transform t;
	t.translate(playAreaX, playAreaY);

	if (boss)
	{
		Text phasesLeft;
		phasesLeft.setString(fmt::format("{}", boss->getPhasesAmount() - boss->getPhase() + 1));
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
			front.setSize({ ww * boss->getHp() / boss->getMaxHp(), hh });
			front.setPosition(xx, yy);
			front.setFillColor({ 255, 0, 0 });
			target.draw(front, t);
		}

		Text timer;
		timer.setPosition(playAreaW, 0.0f);
		timer.setString(fmt::format("{}", std::ceil(boss->getTimer())));
		timer.align(Text::HAlign::Right, Text::VAlign::Top);
		target.draw(timer, t);

		Text name;
		name.setPosition(0.0f, 16.0f);
		name.setStyle(sf::Text::Italic);
		name.setString(boss->getName());
		target.draw(name, t);

		Text spellcardName;
	}

	t.translate(playAreaW + m_hudX, m_hudY);

	// sidebar
	sf::Sprite sidebar;
	sidebar.setTexture(hud);
	sidebar.setTextureRect({ 0, 0, 64, 144 });
	target.draw(sidebar, t);
	
	std::string power;
	if (player.getPower() >= player.getPowerMax())
		power = "MAX";
	else
		power = fmt::format("{}", player.getPower());

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
		player.getScore(),
		player.getLives(),
		player.getBombs(),
		power,
		player.getGraze(),
		player.getPoint(), player.getPointNext()));
	target.draw(hud, t);
}
