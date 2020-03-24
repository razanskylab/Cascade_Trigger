function Clear_Serial_Input(cc)

	while cc.S.BytesAvailable()
		fscanf(cc.S);
	end
end