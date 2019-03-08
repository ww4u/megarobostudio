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
		<Item Name="sub_device2.vi" Type="VI" URL="../sub_device2.vi"/>
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
			<Item Name="sub_device2" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{0BCCBCEB-0C4A-45F3-BAFC-A4A44899D9C1}</Property>
				<Property Name="App_INI_GUID" Type="Str">{6476DD80-B8C4-42DD-A66C-DE6C1E0EC84D}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{44E3170A-45D9-46F3-9104-384C644AC028}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">sub_device2</Property>
				<Property Name="Bld_defaultLanguage" Type="Str">ChineseS</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../builds/NI_AB_PROJECTNAME/sub_device2</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{9A7C56BA-8632-41C0-915C-6D6CC8266048}</Property>
				<Property Name="Bld_version.build" Type="Int">1</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">sub_device2.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../builds/NI_AB_PROJECTNAME/sub_device2/sub_device2.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">支持目录</Property>
				<Property Name="Destination[1].path" Type="Path">../builds/NI_AB_PROJECTNAME/sub_device2/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_actXinfo_enumCLSID[0]" Type="Str">{2EC44EFA-3B5D-4F61-A05D-B2ABF78095D1}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[1]" Type="Str">{83393765-F918-42C3-B491-AFFF7D20486A}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[10]" Type="Str">{BFEB8FBF-506B-4796-89D0-E2D1F945909A}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[11]" Type="Str">{8173A5F8-26F1-4366-8A68-61DD07FD6612}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[12]" Type="Str">{825E976C-AD3F-4CE9-BAC3-8D71108E5BE1}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[13]" Type="Str">{872C08DE-BD89-4C23-89E3-59A87689AE57}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[14]" Type="Str">{44C7A01C-156F-438A-B924-DC6707895BDD}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[15]" Type="Str">{6E61364B-B9D1-4657-A5AE-99233BC8B45C}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[16]" Type="Str">{5A462492-E8A5-4A80-B501-47B29951C230}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[2]" Type="Str">{A8E40549-AD3B-4173-92B0-4354E49B94B2}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[3]" Type="Str">{5C7BC6E5-26B1-485A-B17D-19B15E5A9863}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[4]" Type="Str">{905C637D-0D0A-4415-9D1E-3C97324C0A93}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[5]" Type="Str">{B682EA49-8489-4CF4-96FB-4BAAAE7ACBB9}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[6]" Type="Str">{A70850AE-484A-4074-865A-AEDEF2055C85}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[7]" Type="Str">{E1559D10-811A-402D-9C15-326D4E4360C7}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[8]" Type="Str">{D7EB6DF6-88EF-430E-8FED-DFB9506B4282}</Property>
				<Property Name="Exe_actXinfo_enumCLSID[9]" Type="Str">{9B3BCA0C-CFE4-42D1-9897-72209F161185}</Property>
				<Property Name="Exe_actXinfo_enumCLSIDsCount" Type="Int">17</Property>
				<Property Name="Exe_actXinfo_majorVersion" Type="Int">5</Property>
				<Property Name="Exe_actXinfo_minorVersion" Type="Int">5</Property>
				<Property Name="Exe_actXinfo_objCLSID[0]" Type="Str">{2821EB61-7A73-4359-B3A4-323D094C20C1}</Property>
				<Property Name="Exe_actXinfo_objCLSID[1]" Type="Str">{E2A744BE-F593-4975-A3CC-C1CF31F930B7}</Property>
				<Property Name="Exe_actXinfo_objCLSID[10]" Type="Str">{0915AE5C-6A09-46CA-9437-785295B0E5F7}</Property>
				<Property Name="Exe_actXinfo_objCLSID[11]" Type="Str">{90C1EA5D-FEE1-440D-9C76-2D2E556202FC}</Property>
				<Property Name="Exe_actXinfo_objCLSID[12]" Type="Str">{E3C7A262-BC4E-421E-9B9E-C6AD222F6ABE}</Property>
				<Property Name="Exe_actXinfo_objCLSID[13]" Type="Str">{6FC1F55D-032B-4A19-BEFC-519E7920AB7A}</Property>
				<Property Name="Exe_actXinfo_objCLSID[2]" Type="Str">{99E747E0-093A-4C2D-B95D-241A004EF16C}</Property>
				<Property Name="Exe_actXinfo_objCLSID[3]" Type="Str">{71833E99-ED68-472D-9510-FD42A40CDC25}</Property>
				<Property Name="Exe_actXinfo_objCLSID[4]" Type="Str">{4F8C07AA-9F7D-403D-8C1C-03E23E3E7DD1}</Property>
				<Property Name="Exe_actXinfo_objCLSID[5]" Type="Str">{98434A80-FFC1-44A4-AAC0-17AC63378DB1}</Property>
				<Property Name="Exe_actXinfo_objCLSID[6]" Type="Str">{D9193FF0-A89E-40AF-BD3C-5F7CDAE2B0F0}</Property>
				<Property Name="Exe_actXinfo_objCLSID[7]" Type="Str">{047B73AE-92DD-4778-AAC8-137889CC0550}</Property>
				<Property Name="Exe_actXinfo_objCLSID[8]" Type="Str">{75D4DAE3-B21B-48F6-BCA8-18D93A735E1B}</Property>
				<Property Name="Exe_actXinfo_objCLSID[9]" Type="Str">{5BC4D9B7-F80B-41A6-83EF-39551C486EF9}</Property>
				<Property Name="Exe_actXinfo_objCLSIDsCount" Type="Int">14</Property>
				<Property Name="Exe_actXinfo_progIDPrefix" Type="Str">SubDevice2</Property>
				<Property Name="Exe_actXServerName" Type="Str">SubDevice2</Property>
				<Property Name="Exe_actXServerNameGUID" Type="Str"></Property>
				<Property Name="Source[0].itemID" Type="Str">{157CF03E-CEA3-49E2-A141-8F140CEA2419}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/我的电脑/sub_device2.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">sub_device2</Property>
				<Property Name="TgtF_internalName" Type="Str">sub_device2</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">版权 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">sub_device2</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{0B427A42-3C70-4928-9690-F6DCB2ED28A1}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">sub_device2.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
		</Item>
	</Item>
</Project>
