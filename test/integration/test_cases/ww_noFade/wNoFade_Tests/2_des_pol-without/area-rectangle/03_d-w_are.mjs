import { data } from '../../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
      data: Object.assign(data, {
            filter: record =>
                record.Country == 'Austria' ||
                record.Country == 'Belgium' ||
                record.Country == 'Bulgaria' ||
                record.Country == 'Cyprus' ||
                record.Country == 'Czechia' ||
                record.Country == 'Denmark' ||
                record.Country == 'Estonia' ||
                record.Country == 'Greece' ||
                record.Country == 'Germany' ||
                record.Country == 'Spain' ||
                record.Country == 'Finland' ||
                record.Country == 'France' ||
                record.Country == 'Croatia' ||
               record.Country == 'Hungary'
        }),,
        config: {
            channels: {
                x: 'Year',
                y: ['Country', 'Value 2 (+)'],
                color: 'Country'
            },
            title: 'Stacked Area Chart',
            geometry: 'area'
        } 
    }),

    chart => chart.animate({
        config: {
            channels: {
                x: null,
                y: null ,
                color: 'Country',
                noop: 'Year',
                size: ['Year', 'Value 3 (+)']
            },
            title: 'Change Geoms & coordSys.',
            geometry: 'rectangle'
        }
    }
    )];

export default testSteps;