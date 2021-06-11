import Vizzu from "./vizzu.js";

class VizzuChart extends HTMLElement {
  constructor() {
    super();

    const shadow = this.attachShadow({mode: "open"});
    shadow.innerHTML = `
      <style>
        :host {
          display: block;
        }
        #vizzucanvas {
          width: 100%;
          height: 100%;
        }
      </style>
      <canvas id="vizzucanvas"></canvas>
    `;

    this.chart = new Vizzu(this._canvas);
  }

  get _canvas() {
    return this.shadowRoot.getElementById("vizzucanvas");
  }
}

window.customElements.define("vizzu-chart", VizzuChart);

export default VizzuChart;
