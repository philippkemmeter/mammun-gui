<?php
interface IGSS {	// Interface Getter-Setter-Singleton
	public static function get_instance($id);
	public static function set_instance($id, $data);
}
?>