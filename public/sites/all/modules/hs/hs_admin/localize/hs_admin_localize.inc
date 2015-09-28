<?php

class HSStaticFieldOrder extends SelectQueryExtender
{
    protected $field_order = array();
    protected $field_default_value;

    public function __construct(SelectQueryInterface $query, DatabaseConnection $connection)
    {
        parent::__construct($query, $connection);

        $this->addTag('hs_static_field_order');
    }

    public function setFieldOrder(array $field_order, $default_value = 'NULL')
    {
        $this->field_order = $field_order;
        $this->field_default_value = $default_value;

        return $this;
    }

    public function clearFieldOrder()
    {
        $this->field_order = array();
    }

    public function __toString()
    {
        $sql = parent::__toString();

        // If no specific field order is set, just return the original SQL query.
        if (empty($this->field_order)) {
            return $sql;
        }

        // Determine beginning and end of the concatenated string of column names.
        $query_prefix = 'SELECT ';
        // @todo Check if query is DISTINCT or not.
        /*if ($this->distinct) {
            $query_prefix .= 'DISTINCT ';
        }*/

        $select_expr_start = strlen($query_prefix);
        $select_expr_end = strpos($sql, 'FROM');
        $select_expr_length = $select_expr_end - $select_expr_start - 1;

        // Generate a new version of this string, respecting the order.
        $new_select_expr = $this->generateFieldOrder();

        // Update the select expression in the original SQL query.
        $sql = substr_replace($sql, $new_select_expr, $select_expr_start, $select_expr_length);

        return $sql;
    }

    protected function generateFieldOrder()
    {
        // Start of slightly modified version of SelectQuery::__toString() source code...
         
        $fields = array();
        foreach ($this->getTables() as $alias => $table) {
            if (!empty($table['all_fields'])) {
                $fields[] = $this->connection->escapeTable($alias) . '.*';
            }
        }

        foreach ($this->getFields() as $alias => $field) {
            // Always use the AS keyword for field aliases, as some
            // databases require it (e.g., PostgreSQL).
            $fields[$alias] = (isset($field['table']) ? $this->connection->escapeTable($field['table']) . '.' : '') . $this->connection->escapeField($field['field']) . ' AS ' . $this->connection->escapeAlias($field['alias']);
        }

        foreach ($this->getExpressions() as $alias => $expression) {
            $fields[$alias] = $expression['expression'] . ' AS ' . $this->connection->escapeAlias($expression['alias']);
        }

        // ... End of source code copy.
        
        
        $field_keys = array_flip($this->field_order);

        // Default the missing fields to a default value.
        $missing_fields = array_diff_key($field_keys, $fields);
        foreach ($missing_fields as $alias => $void) {
            $fields[$alias] = $this->field_default_value . ' AS ' . $this->connection->escapeAlias($alias);
        }        

        // Sort the available fields and expressions using the set field order.
        $sorted_fields = array_merge($field_keys, $fields);
        $sorted_fields_string = implode(', ', $sorted_fields);

        return $sorted_fields_string;
    }
}
