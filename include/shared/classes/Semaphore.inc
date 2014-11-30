<?php
/**
 * Diese Klasse verwaltet Semaphore.
 *
 * @author Philipp Kemmeter
 */
class Semaphore {
	protected $sem_id = NULL;

	/**
	 * Erzeugt einen neuen Semaphor.
	 *
	 * @param uint $key			Eindeutiger Schlüssel.
	 * @param uint $max_acquire	Wie viele maximal zugreifen dürfen.
	 */
	public function __construct($key, $max_acquire=1) {
		ValueChecker::int($key, 'key', 1);
		ValueChecker::int($max_acquire, 'max_acquire', 1);

		$this->sem_id = sem_get($key, $max_acquire, 0600, -1);
	}

	/**
	 * Releast den Semaphor.
	 */
	public function __destruct() {
		try {
			@$this->release();
		}
		catch (Exception $e) {
			// Dann ist er schon releast worden, kein Problem.
		}
	}

	/**
	 * Fordert den Zugriff auf den Semaphor an.
	 *
	 * @throws Exception			Falls beim Zugriff ein Fehler passiert.
	 */
	public function acquire() {
		if (!sem_acquire($this->sem_id))
			throw new Exception(
				"Could not acquire semaphore for ID {$this->sem_id}"
			);
	}

	/**
	 * Releaset den Zugriff
	 *
	 * @throws Exception			Falls beim Release ein Fehler passiert.
	 */
	public function release() {
		if (!sem_release($this->sem_id))
			throw new Exception(
				"Could not release semaphore for ID {$this->sem_id}"
			);

	}

	/**
	 * Löscht den Semaphor.
	 *
	 * @throws Exception			Falls beim Löschen ein Fehler passiert.
	 */
	public function remove() {
		if (!sem_remove($this->sem_id))
			throw new Exception(
				"Could not removed semaphore for ID {$this->sem_id}"
			);
	}

}
?>