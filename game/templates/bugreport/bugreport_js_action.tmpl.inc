<? foreach (Bugreport::get_current_attachments() as $attach): ?>
upload_done<?=$this_id?>(1, '<?=$attach->name?>', '<?= strlen($attach->data)?>');
<? endforeach;?>