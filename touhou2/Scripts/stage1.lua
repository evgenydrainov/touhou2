local YoumuNonspell1 = {
	Hp = 100,
	Time = 40,
	Script = function(self)
		while true do
			local dir = point_direction(self.x, self.y, Player.x, Player.y)
			
			local startx = self.x + lengthdir_x(50, dir)
			local starty = self.y + lengthdir_y(50, dir)
			
			local endx = startx - lengthdir_x(100, dir)
			local endy = starty - lengthdir_y(100, dir)
			
			local i
			for i = 1, 10 do
				local x = lerp(startx, endx, i / 10)
				local y = lerp(starty, endy, i / 10)
				
				local n = 5 - math.abs(5 - i)
				
				CreateBullet(x + lengthdir_x(20 * n, dir + 90), y + lengthdir_y(20 * n, dir + 90), 0, dir, 3)
				CreateBullet(x + lengthdir_x(20 * n, dir - 90), y + lengthdir_y(20 * n, dir - 90), 0, dir, 3)
				
				Wait(15)
			end
			
			Wait(120)
		end
	end
}

local Youmu = {
	Name = 'Youmu Konpaku',
	Phases = {
	[0]=YoumuNonspell1
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
