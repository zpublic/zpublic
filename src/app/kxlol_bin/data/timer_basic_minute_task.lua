
function dowork()
	player.add_shenli(1)
	--gui.output("吸收宇宙能量，获得1点空虚神力")
	
	local n = player.get_res_num("生命星球")
	player.add_res_num("轮回者", n*10)
	--gui.output("生命星球有"..n*10.."个轮回者脱颖而出")
	
	gui.update_gui()
	return 0
end