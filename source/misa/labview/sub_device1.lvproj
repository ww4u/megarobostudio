<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="17008000">
	<Item Name="我的电脑" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">我的电脑/VI服务器</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">我的电脑/VI服务器</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="sub_device1.vi" Type="VI" URL="../sub_device1.vi"/>
		<Item Name="依赖关系" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="Acquire Semaphore.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Acquire Semaphore.vi"/>
				<Item Name="AddNamedSemaphorePrefix.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/AddNamedSemaphorePrefix.vi"/>
				<Item Name="Error Cluster From Error Code.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Cluster From Error Code.vi"/>
				<Item Name="GetNamedSemaphorePrefix.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/GetNamedSemaphorePrefix.vi"/>
				<Item Name="Not A Semaphore.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Not A Semaphore.vi"/>
				<Item Name="Obtain Semaphore Reference.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Obtain Semaphore Reference.vi"/>
				<Item Name="Release Semaphore Reference.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Release Semaphore Reference.vi"/>
				<Item Name="Release Semaphore.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Release Semaphore.vi"/>
				<Item Name="RemoveNamedSemaphorePrefix.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/RemoveNamedSemaphorePrefix.vi"/>
				<Item Name="Semaphore RefNum" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Semaphore RefNum"/>
				<Item Name="Semaphore Refnum Core.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Semaphore Refnum Core.ctl"/>
				<Item Name="Validate Semaphore Size.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Validate Semaphore Size.vi"/>
			</Item>
			<Item Name="_get_int.vi" Type="VI" URL="../_get_int.vi"/>
			<Item Name="_get_str.vi" Type="VI" URL="../_get_str.vi"/>
			<Item Name="_miopen.vi" Type="VI" URL="../_miopen.vi"/>
			<Item Name="_set.vi" Type="VI" URL="../_set.vi"/>
			<Item Name="miclose.vi" Type="VI" URL="../miclose.vi"/>
			<Item Name="miopen.vi" Type="VI" URL="../miopen.vi"/>
			<Item Name="robo_call.vi" Type="VI" URL="../robo_call.vi"/>
			<Item Name="robo_getState.vi" Type="VI" URL="../robo_getState.vi"/>
			<Item Name="robo_waitIdle.vi" Type="VI" URL="../robo_waitIdle.vi"/>
			<Item Name="robo_waitx.vi" Type="VI" URL="../robo_waitx.vi"/>
		</Item>
		<Item Name="程序生成规范" Type="Build">
			<Item Name="sub_device1" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{24D57628-7A57-4FC1-8678-D7E01BDC8E96}</Property>
				<Property Name="App_INI_GUID" Type="Str">{38FE1737-326A-4ECA-8DDB-4173C9936A73}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{2871C644-C4ED-4989-AEAF-094596E3B63A}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">sub_device1</Property>
				<Property Name="Bld_defaultLanguage" Type="Str">ChineseS</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../builds/NI_AB_PROJECTNAME/sub_device1</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{FEBB2211-3C74-4B13-8F0F-7A4BE0CA8BD5}</Property>
				<Property Name="Bld_version.build" Type="Int">1</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">sub_device1.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../builds/NI_AB_PROJECTNAME/sub_device1/sub_device1.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">支持目录</Property>
				<Property Name="Destination[1].path" Type="Path">../builds/NI_AB_PROJECTNAME/sub_device1/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_actXinfo_enumCLSID[0]" Type="Str">{51C57443-643F-4B74-8AAD-61D53B799D4D}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[1]" Type="Str">{EE58CC1C-BF25-4151-8D01-C59C529664DE}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[10]" Type="Str">{1B5ED537-2F02-470A-BBA5-D890D55EF272}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[11]" Type="Str">{C2B9CAF5-72F2-4291-AEDF-FF2BD3E313C6}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[12]" Type="Str">{2E23C69E-6A13-4812-8C7E-9AB43C5671FC}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[13]" Type="Str">{3D26D3E6-5728-4A86-A8C3-BD6577B6CAAF}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[14]" Type="Str">{44D0D97C-3F03-48F0-9B4A-C16D0FF50047}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[15]" Type="Str">{D72C22B7-8AA1-4CCB-BD30-69CF42DE26CB}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[16]" Type="Str">{AC5994DA-1CB9-44C2-8271-A6FCEEFC097B}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[2]" Type="Str">{828BB568-1F3C-4D56-84E9-00E69435B015}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[3]" Type="Str">{B7436D5E-A8C0-4881-8B15-0EDE6025514E}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[4]" Type="Str">{945FBF81-5599-4785-A9DB-7F4F8AF583C2}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[5]" Type="Str">{93D6E6C8-6755-464C-91F7-39F84DE85520}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[6]" Type="Str">{19729657-7D89-44BB-AFF9-5DE4D79CC5CB}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[7]" Type="Str">{E21C3D77-D40F-4E98-A66D-E3E72B4F4F5B}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[8]" Type="Str">{E9119D6E-A3F5-483A-BD04-C27E7CC059A0}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[9]" Type="Str">{705E6124-5E5B-49B1-BD62-11D3D0F87510}</Property>
				<Property Name="Exe_actXinfo_enumCLSIDsCount" Type="Int">17</Property>
				<Property Name="Exe_actXinfo_majorVersion" Type="Int">5</Property>
				<Property Name="Exe_actXinfo_minorVersion" Type="Int">5</Property>
				<Property Name="Exe_actXinfo_objCLSID[0]" Type="Str">{69E8D19E-73F1-4A8F-A8CF-BF9C000AC395}</Property>
				<Property Name="Exe_actXinfo_objCLSID[1]" Type="Str">{8E135661-F05F-4159-9092-F68FC1A81546}</Property>
				<Property Name="Exe_actXinfo_objCLSID[10]" Type="Str">{AB2C5052-E9EA-4CE8-80DE-B8B838B8F643}</Property>
				<Property Name="Exe_actXinfo_objCLSID[11]" Type="Str">{491E7FE5-2F5B-4A19-8317-6CE19B59449F}</Property>
				<Property Name="Exe_actXinfo_objCLSID[12]" Type="Str">{6AB8F65B-D77A-4769-88A5-BE1A7B36D085}</Property>
				<Property Name="Exe_actXinfo_objCLSID[13]" Type="Str">{DF48728E-6122-4F70-99DE-6E453B00866B}</Property>
				<Property Name="Exe_actXinfo_objCLSID[2]" Type="Str">{BCFF1BDD-445E-4DED-BA76-A0075C58E7D6}</Property>
				<Property Name="Exe_actXinfo_objCLSID[3]" Type="Str">{A2BF2123-64CC-4831-82A0-607E21CCE893}</Property>
				<Property Name="Exe_actXinfo_objCLSID[4]" Type="Str">{3C111769-C0F2-40CC-9335-E9F53101B48F}</Property>
				<Property Name="Exe_actXinfo_objCLSID[5]" Type="Str">{5BB0EAC5-AF70-4491-A946-4A35ED547982}</Property>
				<Property Name="Exe_actXinfo_objCLSID[6]" Type="Str">{7B7C851F-524D-4DBC-889A-8D5AF3E63944}</Property>
				<Property Name="Exe_actXinfo_objCLSID[7]" Type="Str">{1BC4634C-DB42-43A0-90C4-5E5CDDFE970D}</Property>
				<Property Name="Exe_actXinfo_objCLSID[8]" Type="Str">{C651F504-5695-43F9-B358-CD9EEA0C3A53}</Property>
				<Property Name="Exe_actXinfo_objCLSID[9]" Type="Str">{32B0C582-50F3-41F6-9ED1-DF0EA7D652C3}</Property>
				<Property Name="Exe_actXinfo_objCLSIDsCount" Type="Int">14</Property>
				<Property Name="Exe_actXinfo_progIDPrefix" Type="Str">SubDevice1</Property>
				<Property Name="Exe_actXServerName" Type="Str">SubDevice1</Property>
				<Property Name="Exe_actXServerNameGUID" Type="Str"></Property>
				<Property Name="Source[0].itemID" Type="Str">{32623982-4DED-459C-8FED-53DFF6E3C49F}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/我的电脑/sub_device1.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">sub_device1</Property>
				<Property Name="TgtF_internalName" Type="Str">sub_device1</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">版权 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">sub_device1</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{C61BD235-672C-47B5-A1D9-F1BC3B7463C1}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">sub_device1.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
		</Item>
	</Item>
</Project>
