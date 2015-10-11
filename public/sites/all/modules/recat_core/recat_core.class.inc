<?php

class RecatCorePager extends PagerDefault {

    private $current_page;

    public function execute() {
        if (!$this->preExecute($this)) {
            return NULL;
        }

        if (empty($this->limit)) {
            return;
        }

        $this->ensureElement();
        $total_items = $this->getTotalItemCount();

        $this->current_page = pager_default_initialize($total_items, $this->limit, $this->element);
        $this->range($this->current_page * $this->limit, $this->limit);

        return $this->query->execute();
    }

    public function getTotalItemCount() {
        static $total_items = NULL;

        if ($total_items === NULL) {
            $total_items = $this->getCountQuery()->execute()->fetchField();
        }

        return $total_items;
    }

    public function getTotalPages() {
        if ($this->limit === 0) {
            return 0;
        }

        $total_items = $this->getTotalItemCount();
        return ceil($total_items / $this->limit);
    }

    public function getCurrentPage() {
        return $this->current_page;
    }

}