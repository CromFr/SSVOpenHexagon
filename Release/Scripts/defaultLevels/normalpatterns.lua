-- include useful files
execScript("utils.lua")
execScript("common.lua")
execScript("commonpatterns.lua")

-- this function adds a pattern to the timeline based on a key
function addPattern(mKey)
		if mKey == 0 then pAltBarrage(math.random(2, 4), 2) 
	elseif mKey == 1 then pMirrorSpiral(math.random(3, 6), 0)
	elseif mKey == 2 then pBarrageSpiral(math.random(0, 3), 1, 1)
	elseif mKey == 3 then pBarrageSpiral(math.random(0, 2), 1.2, 2)
	elseif mKey == 4 then pBarrageSpiral(2, 0.7, 1)
	elseif mKey == 5 then pInverseBarrage(0)
	elseif mKey == 6 then pTunnel(math.random(1, 3))
	elseif mKey == 7 then pMirrorWallStrip(1, 0)
	end
end

-- shuffle the keys, and then call them to add all the patterns
-- shuffling is better than randomizing - it guarantees all the patterns will be called
keys = { 0, 0, 1, 1, 2, 2, 3, 4, 5, 5, 6, 7, 7 }
keys = shuffle(keys)
index = 0

-- onLoad is an hardcoded function that is called when the level is started/restarted
function onLoad()
	log("level onLoad")
end

-- onStep is an hardcoded function that is called when the level timeline is empty
-- onStep should contain your pattern spawning logic
function onStep()
	log("level onStep")
	
	addPattern(keys[index])
	index = index + 1
	
	if index == table.getn(keys) then
		index = 0
	end
end

-- onUnload is an hardcoded function that is called when the level is closed/restarted
function onUnload()
	log("level onUnload")	
end