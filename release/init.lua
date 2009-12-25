plist = {}
result = {}

local search = {}
local trace = switch.trace
local activate = switch.activate
local format = string.format
local debug = function(...)
	trace(format(...))
end

dofile("convert.lua")


function issinglematch(search, program)
	local upper = program:upper()
	for word in search:gmatch("[^ ]*") do
		upper_word = word:upper()
		if not upper:find(upper_word) then
			return false
		end
	end
	return true
end

function ismatch(search, abstracts)
	for _, abstract in ipairs(abstracts) do
		if issinglematch(search, abstract) then
			return true
		end
	end
	return false
end

local search = {}

function onsearchtextchange(text)
	if not plist.old then
		local idx = 1
		local key = 1
		search = {}
		while plist[idx] do
			local item = {}

			item.key = key

			item.program = plist[idx]
			idx = idx + 1

			item.abstracts = {}
			table.insert(item.abstracts, item.program)
			local pinyins = convert(item.program)
			for _, v in ipairs(pinyins) do
				table.insert(item.abstracts, v)
			end

			item.hwnd = plist[idx]
			idx = idx + 1

			search[key] = item
			key = key + 1
		end
		plist.old = true
	end

	result = {}

	for i, item in ipairs(search) do
		if ismatch(text, item.abstracts) then
			table.insert(result, item)
		end
	end
end

function onok()
	if result[1] and result[1].hwnd then
		activate(result[1].hwnd)
	end
end


