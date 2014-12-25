
function build()
	if player.get_shenli() >= 9 then
		player.sub_shenli(9)
		player.add_res_num("轮回神殿", 1);
		gui.update_gui()
		gui.messagebox("建造成功！消耗⑨点神力", "")
		return 0
	else
		gui.messagebox("神力不够！需要⑨点神力", "")
	end
	return -1
end