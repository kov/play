var vSText =
[
'precision mediump float;',
'attribute vec2 vertPosition;',
'attribute vec3 vertColor;',
'varying vec3 fragColor;',
'void main()',
'{',
'   fragColor = vertColor;',
'   gl_Position = vec4(vertPosition, 0.0, 1.0);',
'}'
].join('\n');

var fSText =
[
'precision mediump float;',
'varying vec3 fragColor;',
'void main()',
'{',
'   gl_FragColor = vec4(fragColor, 1.0);',
'}'
].join('\n');

function initGL() {
    console.log('initGL');
    var canvas = document.getElementById('surface');
    var gl = canvas.getContext('webgl');

    gl.clearColor(0.75, 0.5, 0.5, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    function buildShader(type, text) {
        var shader = gl.createShader(type);
        gl.shaderSource(shader, text);
        gl.compileShader(shader);

        if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
            console.error('Error compiling shader: ' + text, gl.getShaderInfoLog(shader));
            return null;
        }
        return shader;
    }

    var program = gl.createProgram();

    var shader = buildShader(gl.VERTEX_SHADER, vSText);
    gl.attachShader(program, shader);

    shader = buildShader(gl.FRAGMENT_SHADER, fSText);
    gl.attachShader(program, shader);

    gl.linkProgram(program);
    if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
        console.error('Error linking program: ', gl.getProgramInfoLog(program));
        return;
    }

    // Buffer
    var vertices = new Float32Array([
        0.0, 0.5, 1.0, 0.0, 0.0,
        -0.5, -0.5, 0.0, 1.0, 0.0,
        0.5, -0.5, 0.0, 0.0, 1.0
    ]);

    var bufferObject = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, bufferObject);
    gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);

    var positionAttribLocation = gl.getAttribLocation(program, 'vertPosition');
    gl.vertexAttribPointer(
        positionAttribLocation,
        2,
        gl.FLOAT,
        gl.FALSE,
        5 * Float32Array.BYTES_PER_ELEMENT,
        0
    );

    gl.enableVertexAttribArray(positionAttribLocation);

    var colorAttribLocation = gl.getAttribLocation(program, 'vertColor');
    gl.vertexAttribPointer(
        colorAttribLocation,
        3,
        gl.FLOAT,
        gl.FALSE,
        5 * Float32Array.BYTES_PER_ELEMENT,
        2 * Float32Array.BYTES_PER_ELEMENT
    );

    gl.enableVertexAttribArray(colorAttribLocation);

    // Render
    gl.useProgram(program);
    gl.drawArrays(gl.TRIANGLES, 0, 3);
}
