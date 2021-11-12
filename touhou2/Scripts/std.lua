--Width 			= 384
--Height 			= 448

function round(x)
	return x + (2^52 + 2^51) - (2^52 + 2^51)
end

function Wait(t)
	while t > 0 do
		t = t - 1
		coroutine.yield()
	end
end

function dsin(x) return math.sin(math.rad(x)) end

function dcos(x) return math.cos(math.rad(x)) end

function lengthdir_x(l, d) return l*dcos(d) end

function lengthdir_y(l, d) return l*-dsin(d) end

function clamp(v, lo, hi) return math.max(math.min(v, hi), lo) end

function approach(a, b, n) return a + clamp(b-a, -n, n) end

function sqr(x) return x*x end

function point_direction(x1, y1, x2, y2) return math.deg(math.atan(y1-y2, x2-x1)) end

function point_distance(x1, y1, x2, y2) return math.sqrt(sqr(x2-x1) + sqr(y2-y1)) end

function lerp(a, b, t) return a + (b-a)*t end

function floor_to(x, n) return math.floor(x/n) * n end

function round_to(x, n) return round(x/n) * n end

function ceil_to(x, n) return math.ceil(x/n) * n end

--function launch_towards(inst, target_x, target_y, acc)
--	inst.speed = math.sqrt(point_distance(inst.x, inst.y, target_x, target_y) * math.abs(acc) * 2)
--	inst.direction = point_direction(inst.x, inst.y, target_x, target_y)
--	inst.acc = acc
--end

function slide_to(inst, target_x, target_y, t)
	while math.abs(target_x-inst.x)>.1 and math.abs(target_y-inst.y)>.1 do
		inst.x = lerp(inst.x, target_x, t)
		inst.y = lerp(inst.y, target_y, t)
		Wait(1)
	end
end
