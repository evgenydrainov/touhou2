function Wait(t)
	while t > 0 do
		t = t - 1
		coroutine.yield()
	end
end

function dsin(x) return math.sin(math.rad(x)) end

function dcos(x) return math.cos(math.rad(x)) end

function sqr(x) return x*x end

function lerp(a, b, t) return a + (b-a)*t end

function clamp(v, lo, hi) return math.max(math.min(x, hi), lo) end

function approach(a, b, t) return a + clamp(b-a, -t, t) end

function point_direction(x1, y1, x2, y2) return math.deg(math.atan(y1-y2, x2-x1)) end

function point_distance(x1, y1, x2, y2) return math.sqrt(sqr(x2-x1) + sqr(y2-y1)) end

function lengthdir_x(l, d) return l*dcos(d)  end

function lengthdir_y(l, d) return l*-dsin(d) end
