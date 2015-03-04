local function need_shenli()
	local n = player.get_res_num("原始位面", "生命星球")
	local i = 0
	local need = 2
	while i < n do
		need = need * 2
		i = i + 1
	end
	return need
end

function build()
	local need = need_shenli()
	if player.get_shenli() >= need then
		player.sub_shenli(need)
		player.add_res_num("原始位面", "生命星球", 1);
		gui.update_gui()
		gui.messagebox("建造成功！消耗"..need.."点神力", "")
		return 0
	else
		gui.messagebox("神力不够！需要"..need.."点神力", "")
	end
	return -1
end