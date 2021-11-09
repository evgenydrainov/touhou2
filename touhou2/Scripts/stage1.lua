PHASE_NONSPELL			= 1
PHASE_SPELLCARD 		= 2

function Wait(t)
	while t > 0 do
		t = t - 1
		coroutine.yield()
	end
end

local YoumuNonspell1 = {
	Type = PHASE_NONSPELL,
	Hp = 100,
	Time = 40,
	Script = function(self)
		while true do
			local x = BossGetX(self)
			local y = BossGetY(self)
			CreateBullet(x, y, 5, point_direction(x, y, PlayerGetX(0), PlayerGetY(0)), 3)
			Wait(15)
		end
	end
}

local YoumuNonspell2 = {
	Type = PHASE_NONSPELL,
	Hp = 120,
	Time = 50,
	Script = function(self)
		while true do
			local x = BossGetX(self)
			local y = BossGetY(self)
			CreateBullet(x, y, 6, point_direction(x, y, PlayerGetX(0), PlayerGetY(0)), 4)
			Wait(10)
		end
	end
}

local YoumuSpellcardA = {
	Type = PHASE_SPELLCARD,
	Name = '',
	Background = -1,
	Hp = 110,
	Time = 45,
	Script = function(self)
		while true do
			local x = BossGetX(self)
			local y = BossGetY(self)
			CreateBullet(x, y, 5.5, point_direction(x, y, PlayerGetX(0), PlayerGetY(0)), 3.5)
			Wait(12.5)
		end
	end
}

local YoumuSpellcardB = {
	Type = PHASE_SPELLCARD,
	Name = '',
	Background = -1,
	Hp = 130,
	Time = 55,
	Script = function(self)
		while true do
			local x = BossGetX(self)
			local y = BossGetY(self)
			CreateBullet(x, y, 6.5, point_direction(x, y, PlayerGetX(0), PlayerGetY(0)), 4.5)
			Wait(7.5)
		end
	end
}

local Youmu = {
	Name = 'Youmu Konpaku',
	Phases = {
	[0]=YoumuNonspell1, YoumuSpellcardA,
		YoumuNonspell2, YoumuSpellcardB
	},
	Healthbars = {
	[0]={ [0]= 0, 1 },
		{ [0]= 2, 3 }
	}
}

Stage = {
	Script = function()
		print(123)
		Wait(60)
		print(456)
		CreateBoss(Youmu)
	end
}
