function can_build()
	return true
end

function build()
	if can_build() then
		return 0;
	end
	return -1;
end