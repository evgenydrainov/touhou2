local function slide_to(target_x, target_y, t)
	while point_distance(self.x, self.y, target_x, target_y)>.5 do
		self.x = lerp(self.x, target_x, t)
		self.y = lerp(self.y, target_y, t)
		Wait(1)
	end
end

local function change_delta(targdelta, t)
	while Game.Delta~=targdelta do
		Game.Delta = approach(Game.Delta, targdelta, t)
		Wait(1)
	end
end

local function AnimateYoumu()
	self.w = 48
	self.h = 64
	while true do
		self.u = 0
		self.v = 16
		Wait(20)
		self.u = 48
		self.v = 16
		Wait(20)
		self.u = 48 * 2
		self.v = 16
		Wait(20)
		self.u = 48 * 3
		self.v = 16
		Wait(20)
	end
end

local YoumuNonspell1 = {
	Hp = 1000,
	Time = 60,
	Script = function()
		self:add_thread(AnimateYoumu)
		local function shoot(x, y, dir, uvs)
			local N = 8
			local i
			for i = 0,N-1 do
				local t = i / (N-1)
				CreateBullet(x, y, 1, dir - 90 + 180*t, 4, uvs)
			end
		end
		local function slash(x, y, targx, targy, uvs)
			local N = 12
			local i
			for i = 0,N-1 do
				local xx = x + 20*(i-N/2)
				local yy = y
				local ii
				for ii = 1,4 do
					local b = CreateBullet(xx, yy, 1 + ii, point_direction(xx, yy, targx, targy), 3, uvs)
					b.rotate = true
				end
				Wait(1)
			end
		end
		while true do
			local ii
			for ii = 1,2 do
				local dir = point_direction(self.x, self.y, Player.x, Player.y)
				local startlen = 180
				local startx = self.x + lengthdir_x(startlen, dir)
				local starty = self.y + lengthdir_y(startlen, dir)
				local endlen = 380
				local endx = startx - lengthdir_x(endlen, dir)
				local endy = starty - lengthdir_y(endlen, dir)
				local N = 40
				local i
				for i = 0,N-1 do
					local t = i / (N-1)
					local x = lerp(startx, endx, t)
					local y = lerp(starty, endy, t)
					local off = 30 + 150 * dsin(180*t)
					if i%2 == 0 then
						shoot(x + lengthdir_x(off, dir + 90), y + lengthdir_y(off, dir + 90), dir, {  0, 0, 16, 16 })
					else
						shoot(x + lengthdir_x(off, dir - 90), y + lengthdir_y(off, dir - 90), dir, { 16, 0, 16, 16 })
					end
					Wait(4)
				end
				Wait(180)
			end
			for ii = 1,2 do
				slash(self.x, self.y, Player.x, Player.y, { 32, 0, 16, 16 })
				Wait(50)
				slash(self.x-50, self.y, Player.x-70, Player.y, { 48, 0, 16, 16 })
				slash(self.x+50, self.y, Player.x+70, Player.y, { 48, 0, 16, 16 })
				Wait(60)
			end
		end
	end
}

local GhostSword = {
	Hp = 1000,
	Time = 50,
	Script = function()
		self:add_thread(AnimateYoumu)
		local function bullet()
			local targdir = self.direction + 360
			while self.direction < targdir do
				self.direction = self.direction + 5
				Wait(1)
			end
			while self.speed < 2.5 do
				self.speed = self.speed + .1
				Wait(1)
			end
		end
		local function shoot(x, y, dir)
			local N = 25
			local i
			for i = 0,N-1 do
				local b = CreateBulletB(x, y, 1, dir + 360*i/N, 3, { 32, 0, 16, 16 }, bullet)
				b.rotate = true
			end
		end
		local function slash(xstart, xend, y)
			local N = 15
			local i
			for i = 0,N-1 do
				local t = i / (N-1)
				local x = lerp(xstart, xend, t)
				shoot(x, y, point_direction(x, y, Player.x, Player.y))
				Wait(5)
			end
		end
		slide_to(60, 140, .05)
		while true do
			change_delta(1, 1/60)
			self.x = 384-60
			slash(0, self.x, self.y)
			Wait(120)
			Game.Delta = .25
			Wait(120)
			change_delta(1, 1/60)
			self.x = 60
			slash(384, self.x, self.y)
			Wait(120)
			Game.Delta = .25
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
		Wait(30)
		CreateBoss(Youmu)
	end
}
