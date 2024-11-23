import React, { useEffect, useState } from 'react';
import axios from 'axios';
import { TextField, Button, List, ListItem, ListItemText, Typography, Box, Container } from '@mui/material';

const SearchBox = () => {
  const [query, setQuery] = useState('');
  const [results, setResults] = useState([]);

  const handleSearch = async (e) => {
    e.preventDefault();

    // Define the terms based on user input
    const terms = query.trim().split(/\s+/); // Splits query by whitespace
    axios
      .post('/search', { query: 'query', terms: terms })
      .then((res) => {
        console.log(res.data);
        setResults(res.data.results);
      })
      .catch((error) => alert(error));
  };

  const handleResultClick = (filename) => {
    // Base URL of the external website
    const baseURL = "https://aquamarine-youtiao-33201d.netlify.app";
  
    // Construct the full URL with the filename
    const externalURL = `${baseURL}/${filename}`;
  
    // Navigate to the external URL in the same tab
    window.location.href = externalURL;

  };

  useEffect(() => console.log(results), [results]);

  return (
    <Container maxWidth="sm" style={{ marginTop: '40px', textAlign: 'center' }}>
      <Typography variant="h4" gutterBottom>
        Search App
      </Typography>
      <Box component="form" onSubmit={handleSearch} noValidate sx={{ mb: 3 }}>
        <TextField
          fullWidth
          label="Enter your search terms"
          variant="outlined"
          value={query}
          onChange={(e) => setQuery(e.target.value)}
          sx={{ mb: 2 }}
        />
        <Button
          type="submit"
          variant="contained"
          color="primary"
          size="large"
          fullWidth
        >
          Search
        </Button>
      </Box>
      <Box>
        {results.length > 0 ? (
          <List>
            {results.map((result, index) => (
              <ListItem
                key={index}
                sx={{ borderBottom: '1px solid #ddd', display: 'flex', justifyContent: 'space-between' }}
              >
                <ListItemText
                  primary={result.filename}
                  secondary={`Weight: ${result.weight}`}
                />
                <Button
                  variant="outlined"
                  color="primary"
                  onClick={() => handleResultClick(result.filename)}
                >
                  Open
                </Button>
              </ListItem>
            ))}
          </List>
        ) : (
          <Typography variant="body1" color="textSecondary">
            No results found
          </Typography>
        )}
      </Box>
    </Container>
  );
};

export default SearchBox;