
function dowork()
	player.add_shenli(1)
	gui.info_output("吸收宇宙能量，获得1点空虚神力")
	
	local n = player.get_res_num("生命星球")
	if n > 0 then
		local m = n * 10
		player.add_res_num("轮回者", m)
		gui.info_output("你的"..n.."个生命星球有"..m.."个轮回者脱颖而出")
	end
	
	gui.update_gui()
	return 0
end