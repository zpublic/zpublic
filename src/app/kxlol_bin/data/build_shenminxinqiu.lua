function can_build()
	if player.get_shenli() > 3 then
		return true
	end
	return false
end

function build()
	if can_build() then
		player.sub_shenli(3)
		return 0
	end
	return -1
end