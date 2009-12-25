
local file = io.open("winpy.txt", "r")
local count = 0
local mb = {}
for line in file:lines() do
	local index = line:find("[abcdefghijklmnopqrstuvwxyz]")
	if index == 3 then
		local char = line:sub(1,2)
		local pinyins = line:sub(3, -1)
		local pys = {}
		for pinyin in pinyins:gmatch("[^ ]+") do
			table.insert(pys, pinyin)
		end
		mb[char] = pys
		count = count + 1
	end
end

function convert(chinese)
	for i = 1, #chinese do
		if chinese:byte(i) >= 0xa1 then
			local char = string.char(chinese:byte(i), chinese:byte(i+1))
			local ret = {}
			local before = chinese:sub(1, i-1)
			local after = chinese:sub(i+2, -1)
			if not mb[char] then
				return convert(before .. "	" .. after)
			end
			for _, v in ipairs(mb[char]) do
				local subret = convert(before..v..after)
				for _, vv in ipairs(subret) do
					table.insert(ret, vv)
				end
			end
			return ret
		end
	end
	return { chinese }
end

