CKEDITOR.on('instanceReady', function (ev) {
    ev.editor.filter.addTransformations([['img{width,height}: sizeToStyle', 'img[width,height]: sizeToAttribute']]);
});

CKEDITOR.config.extraAllowedContent = 'dl dt dd';

CKEDITOR.editorConfig = function (config) {
    config.stylesSet = [
        { name: 'Button', element: 'a', attributes: { class: 'btn' } },
        { name: 'Button bordered', element: 'a', attributes: { class: 'btn btn-bordered' } }
    ];
};