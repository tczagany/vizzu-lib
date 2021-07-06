
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
    { task: chart => chart.animate(
        {
            data: testData,
            descriptor : {
                channels: {
                    x: { attach: [ 'Colors'] },
                },
                title: null,
                legend: null,
            }
        }),
        ref: {
            '20%': '',
            '40%': '',
            '60%': '',
            '80%': '',
            '100%': '',
        }},
    { task: chart => chart.animate(
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
        }),
        ref: {
            '20%': '',
            '40%': '',
            '60%': '',
            '80%': '',
            '100%': '',
        }},
    { task: chart => chart.animate(
        {
            descriptor : {
                channels: {
                    color: { attach: [ 'Colors' ]}
                }
            }
        }),
        ref: {
            '20%': '',
            '40%': '',
            '60%': '',
            '80%': '',
            '100%': '',
        }},
    { task: chart => chart.animate(
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
        }),
        ref: {
            '20%': '',
            '40%': '',
            '60%': '',
            '80%': '',
            '100%': '',
        }},
    { task: chart => chart.animate(
        {
            descriptor : {
                channels: {
                    lightness: { detach: [ 'Colors' ]},
                    label: { attach: [ 'Colors' ]}
                }
            }
        }),
        ref: {
            '20%': '',
            '40%': '',
            '60%': '',
            '80%': '',
            '100%': '',
        }},
]


export default { testData, testSteps }
