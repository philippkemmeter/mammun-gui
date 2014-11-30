<?php

/**
 * Diese Struktur bündelt alle JS-Mausereignisse. Jeder Variablen kann ein
 * JS-Statement zugewiesen werden. Diese Struktur ist geeignet zur Übergabe von
 * JS-MouseEvents zwischen Funktionen etc.
 */
class JSMouseEventStruct { // Dies ist als Struktur zu verstehen - alles Public
	public $on_mouse_over;
	public $on_mouse_out;
	public $on_mouse_down;
	public $on_mouse_up;
	public $on_mouse_move;
	public $on_click;
	public $on_dbl_click;
	public $on_context_menu;
}


?>