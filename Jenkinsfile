pipeline {
    agent { docker { image 'epitechcontent/epitest-docker' } }
    stages {
        stage('Build and Test') {
            steps {
                sh 'env'

                configFileProvider ([
                    configFile(
                        fileId: '405fdc1c-5b16-4ef3-a250-e6b6a35ba0a3',
                        targetLocation: '.epitechcli.config.json'
                    )
                ]) {
                    sh 'curl -o EpitechCLI-latest.jar https://blueslime.fr/epitechcli/EpitechCLI-latest.jar; java -jar EpitechCLI-latest.jar test'
                }
            }
        }
    }
    post {
        always {
            cobertura (
                coberturaReportFile: '**/coverage.xml',
                failNoReports: false
            )
            junit (
                testResults: '**/report.xml',
                allowEmptyResults: true
            )
            archiveArtifacts artifacts: 'trace.txt'
        }
    }
}