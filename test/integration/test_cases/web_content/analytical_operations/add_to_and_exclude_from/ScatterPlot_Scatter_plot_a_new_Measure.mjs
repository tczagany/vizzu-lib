import { data_8 } from "../../../../test_data/chart_types_eu.mjs";

const description = `- add a new Measure to the Size channel`;
const testSteps = [
  (chart) =>
    chart.animate({
      data: data_8,

      config: {
        channels: {
          x: "Value 5 (+/-)",
          y: "Value 3 (+)",
          noop: "Country",
          label: "Country",
        },
        
        geometry: "circle",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          size: "Value 2 (+)",
          label: ["Country", "Value 2 (+)"],
        },
        
        legend: "size",
      },
      style: {
        plot: {
          marker: {
            label: {
              format: "dimensionsFirst",
            },
          },
        },
      },
    }),
  (chart) => {
    chart.feature("tooltip", true);
    return chart;
  },
];

export { description };
export default testSteps;
