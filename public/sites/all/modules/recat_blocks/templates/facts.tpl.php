<div class="row">
    <?php foreach ($facts as $fact): ?>
    <div class="columns small-12 medium-6 large-3">
        <div class="fact">
            <div class="number">
                <i class="icon sprite <?php print $fact['class']; ?>"></i>
                <?php print $fact['number']; ?>
            </div>

            <?php print $fact['label']; ?>
        </div>
    </div>
    <?php endforeach; ?>
</div>