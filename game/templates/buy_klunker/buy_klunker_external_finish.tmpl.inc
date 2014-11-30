<script type="text/javascript">
window.opener.ui_klunker_gainer.add_current_amount(<?=$klunker_add?>);
window.opener.ui_refresh();
window.opener.UIButtons.toggle_buy_klunker();
</script>
<div style="background: url(../pix/ui/backgrounds/buy_klunker/final_bg.jpg) top center no-repeat #F2F7FF; width: 100%; height: 100%; ">
	<div style="color: #333; padding: 10px; margin: auto; width:500px">

		<p style="text-align: center; margin-top:0">
			<strong><span style="font-size: 1.5em"><?=
				LNG_BUY_KLUNKER_SUCCESSFUL_TITLE ?></span><br />
			<span style="font-size: 1.3em"><?=
				str_replace(
					'[KKR]', $klunker_add,
					LNG_BUY_KLUNKER_SUCCESSFUL_SUBTITLE)
					?></span></strong>
		</p>
		<p style="text-align: center; padding-top: 190px;">
		<?php
			$numStr = '+' . number_format($klunker_add, 0, ',', '.');
			$numArray = str_split($numStr, 1);
			foreach ($numArray as $num) :
				$alt = $num;

				switch ($num) {
					case '+' :
						$num = 'plus';
						break;
					case '.' :
						$num = 'dot';
						break;
					case ',' :
						$num = 'comma';
						break;

				}
			?><img src="../pix/ui/backgrounds/buy_klunker/number_formatting/<?php echo $num ?>.png" alt="<?php echo $alt ?>" /><?php endforeach;
		?>
		</p>
		<div style="text-align: center; padding-top: 40px;">
			<button onclick="javascript: window.close();"><?=
				LNG_BUY_KLUNKER_B_CLOSE_WINDOW ?></button>
		</div>
	</div>
</div>