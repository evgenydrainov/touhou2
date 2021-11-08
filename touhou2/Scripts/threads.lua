threads = {}

function create_thread(f)
	threads[#threads + 1] = coroutine.create(f)
end

function update_threads()
	local j = 1
	local n = #threads
	for i = 1, n do
		if coroutine.resume(threads[i]) then
			if i ~= j then
				threads[j] = threads[i]
				threads[i] = nil
			end
			j = j + 1
		else
			threads[i] = nil
		end
	end
end
