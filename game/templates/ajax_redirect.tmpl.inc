<?php if (!isset($redirect_page)) $redirect_page = 'ingame.php'?>
<script type="text/javascript">
ui_auto_logout = false;
document.location.href = "<?= $redirect_page ?>";
</script>
...