const queryString = window.location.search;
console.log(queryString);
const urlParams = new URLSearchParams(queryString);
const test_case = urlParams.get('test_case')
console.log(test_case);