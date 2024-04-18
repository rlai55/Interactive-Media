function setup() {
    let canvas = createCanvas(400, 300);
    canvas.parent('sketch-holder');
    background(255);
}

function draw() {
    stroke(0);
    strokeWeight(2);
    if (mouseIsPressed) {
        line(mouseX, mouseY, pmouseX, pmouseY);
    }
}
