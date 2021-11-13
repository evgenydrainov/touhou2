local YoumuNonspell1 = {
	Hp = 100,
	Time = 60,
	Script = function()
		local shoot = function(x, y, dir)
			local N = 8
			local i
			for i = 0,N-1 do
				local t = i / (N-1)
				CreateBullet(x, y, 1, dir - 90 + 180*t, 3)
			end
		end
		while true do
			local dir = point_direction(self.x, self.y, Player.x, Player.y)
			local startlen = 150
			local startx = self.x + lengthdir_x(startlen, dir)
			local starty = self.y + lengthdir_y(startlen, dir)
			local endlen = 300
			local endx = startx - lengthdir_x(endlen, dir)
			local endy = starty - lengthdir_y(endlen, dir)
			local N = 25
			local T = 4
			local i
			for i = 0,N-1 do
				local t = i / (N-1)
				local x = lerp(startx, endx, t)
				local y = lerp(starty, endy, t)
				local off = 30 + 150 * dsin(180*t)
				if i%2 == 0 then
					shoot(x + lengthdir_x(off, dir + 90), y + lengthdir_y(off, dir + 90), dir)
				else
					shoot(x + lengthdir_x(off, dir - 90), y + lengthdir_y(off, dir - 90), dir)
				end
				Wait(T)
			end
			Wait(N*T + 120)
		end
	end
}

local GhostSword = {
	Hp = 100,
	Time = 50,
	Script = function()
		local bullet = function()
			local targdir = self.direction + 360
			while self.direction~=targdir do
				self.direction = approach(self.direction, targdir, 5)
				Wait(1)
			end
		end
		local shoot = function(x, y, dir)
			local N = 10
			local i
			for i = 0,N-1 do
				CreateBulletB(x, y, 1, dir + 360*i/N, -2, bullet)
			end
		end
		local slash = function(xstart, xend)
			local N = 15
			local i
			for i = 0,N-1 do
				local t = i / (N-1)
				local x = lerp(xstart, xend, t)
				shoot(x, self.y, point_direction(x, self.y, Player.x, Player.y))
				Wait(4)
			end
		end
		while true do
			slide_to(self, 60, 140, .05)
			Wait(30)
			self.x = 384-60
			Wait(30)
			slash(0, self.x)
			Wait(120)
		end
	end
}

local Youmu = {
	Name = 'Youmu Konpaku',
	Phases = {
		YoumuNonspell1, GhostSword
	},
	Healthbars = {
		{ 1, 2 }
	}
}

Stage = {
	Script = function()
		print()
		Wait(30)
		CreateBoss(Youmu)
	end
}
