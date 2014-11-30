<p>
Please enter data for two tribes to start the simulation:<br/>
</p>
<form method="POST" action="fight_simulator.php">
	<table width=400>
		<tr>
		 <td>
			Tribe 1:<br/><br/>
			ATT: <input type=text name=att1 size=4 value=<?=$att1?>><br/>
			DEF: <input type=text name=def1 size=4 value=<?=$def1?>><br/>
			HP: <input type=text name=hp1 size=4 value=<?=$hp1?>><br/>
			Cost: <input type=text name=cost1 size=4 value=<?=$cost1*1000?>>
		 </td>
		 <td>
			Tribe 2:<br/><br/>
			ATT: <input type=text name=att2 size=4 value=<?=$att2?>><br/>
			DEF: <input type=text name=def2 size=4 value=<?=$def2?>><br/>
			HP: <input type=text name=hp2 size=4 value=<?=$hp2?>><br/>
			Cost: <input type=text name=cost2 size=4 value=<?=$cost2*1000?>>
		 </td>
		</tr>
		<tr>
		 <td colspan=2>
		 	Cycle length(s): <input type=text name=cycle size=6 value=<?=$cycle?>>
		 		<input type=submit name=submitok value="Calculate now!">
		 </td>
		</tr>
	</table>
</form>