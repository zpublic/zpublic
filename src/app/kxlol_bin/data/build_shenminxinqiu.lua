
function build()
	if player.get_shenli() >= 3 then
		player.sub_shenli(3)
		player.add_res_num("生命星球", 1);
		gui.update_gui()
		gui.messagebox("建造成功！消耗3点神力", "")
		return 0
	else
		gui.messagebox("神力不够！需要3点神力", "")
	end
	return -1
end