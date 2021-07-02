
const testData = {
    series: [
        {
            name: 'Colors',
            type: 'categories',
            values: ['red', 'green', 'blue']
        },
        {
            name: 'Val',
            type: 'values',
            values: [ 3, 5, 4 ]
        }
    ]
};

const testSteps = [
    [chart => chart.animate(
        {
            data: testData,
            descriptor : {
                channels: {
                    x: { attach: [ 'Colors'] },
                },
                title: null,
                legend: null,
            }
        }),'0'],
    [chart => chart.animate(
        {
            data: {
                filter: record => record.Colors != 'blue'
            },
            descriptor : {
                channels: {
                    x: { detach: [ 'Colors'] },
                    y: { attach: [ 'Colors' ]}
                },
            }
        }),'0'],
    [chart => chart.animate(
        {
            descriptor : {
                channels: {
                    color: { attach: [ 'Colors' ]}
                }
            }
        }),'0'],
    [chart => chart.animate(
        {
            data: {
                filter: null
            },
            descriptor : {
                channels: {
                    color: { detach: [ 'Colors' ]},
                    lightness: { attach: [ 'Colors' ]}
                }
            }
        }),'0'],
    [chart => chart.animate(
        {
            descriptor : {
                channels: {
                    lightness: { detach: [ 'Colors' ]},
                    label: { attach: [ 'Colors' ]}
                }
            }
        }),'0'],
]


export default { testData, testSteps }
