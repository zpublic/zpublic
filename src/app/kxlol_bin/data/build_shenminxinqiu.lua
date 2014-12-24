function can_build()
	if player.get_shenli() >= 2 then
		return true
	end
	return false
end

function build()
	if can_build() then
		player.sub_shenli(3)
		gui.update_gui()
		gui.messagebox("shenmegui", "")
		return 0
	end
	return -1
end