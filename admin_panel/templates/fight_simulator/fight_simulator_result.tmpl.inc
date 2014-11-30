<p>
Fight results (every side <?= $food_invested ?> food invested, 
total: <?=(round($total*10)/10)?> units):<br/>
<br/>
</p>
<table>
<tr>
	<td>Tribe 1 attacking (<?=(round($viruses*10)/10)?> units):<br/>
		Unit power: <?=(round($vipo1*1000)/1000)?><br/>
		Powed unit power: <?=(round($vipo11*1000)/1000)?><br/>
		Enemies killed: <?=(round($dead1*1000)/1000)?><br/>
		Food destroyed: <?=(round($bio1*1000)/1000)?>	   
	</td>
	<td>Tribe 2 attacking (<?=(round($enemies*10)/10)?> units):<br/>
		Unit power: <?=(round($vipo2*1000)/1000)?><br/>
		Powed unit power: <?=(round($vipo22*1000)/1000)?><br/>
		Enemies killed: <?=(round($dead2*1000)/1000)?><br/>
		Bio destroyed: <?=(round($bio2*1000)/1000)?>
	</td>
</tr>
</table>
<p>
<? if ($ratio) :?> 
Tribe <?= $winner?> wins with ratio: <?=(round($ratio2*1000)/1000)?> / <?=(round($ratio*1000)/1000)?>
<? else :?>
no tribe wins
<? endif ?>
</p>